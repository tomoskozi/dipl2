#ifndef __NC_RTP_SOURCE_HH__
#define __NC_RTP_SOURCE_HH__


#include <cassert>
#include <memory.h>
#include <cstdio>
#include <deque>
//#include <unordered_set>
#include <forward_list>
#include <thread>

#include <kodo/rlnc/on_the_fly_codes.hpp>
#include <kodo/rlnc/full_vector_codes.hpp>
#include <kodo/rs/reed_solomon_codes.hpp>
#include <kodo/rlnc/sliding_window_encoder.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>
#include <crc.hh>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "RTSPClient.hh"
#include "RTPSource.hh"

RTSPClient *g_pRtspClient;

namespace codelance
{
//#define IN_ORDER_DELIVERY
#define FULL_NC//RS//FULL_NC//ONLINE_NC//SW_NC
#ifdef ONLINE_NC
    typedef kodo::on_the_fly_decoder<fifi::binary8> rlnc_decoder_t;
#endif
#ifdef FULL_NC
    typedef kodo::full_rlnc_decoder<fifi::binary8> rlnc_decoder_t;
#endif
#ifdef RS 
    typedef kodo::rs_decoder<fifi::binary8> rlnc_decoder_t;
#endif
#ifdef SW_NC
    typedef kodo::sliding_window_decoder<fifi::binary8> rlnc_decoder_t;
#endif
	typedef std::vector<uint8_t> packet_t;  //payload_t;
    typedef std::deque<packet_t*> packets_t;
    typedef std::pair<packet_t, std::mutex> sharedPacket_t;

    const uint32_t g_symbols     = 32;//16U;
    const uint32_t g_symbolSize  = 1300;//12U;

    rlnc_decoder_t::factory g_decoderFactory(g_symbols, g_symbolSize);   // Decoder factory

    static void RawToStream(const unsigned char *buf, unsigned short len, std::ostream &out)
    {
        assert(buf);
        for (size_t i = 0; i < len; ++i) 
        {
            out << buf[i];
        }
        printf("\n");
    } /* RawToStream */

    static void write_packet_dumpx(const unsigned char *buf, unsigned short len)
    {
        if (buf != NULL && len != 0) {
            int i;

		    for (i = 0; i < len; i += 8) {
                int j;

                printf("%04x ", i);
                for (j = 0; j < 8 && (i + j) < len; j++) {
                    printf("%02x ", buf[i + j]);
                }
                printf("\n");
            }
	        printf("\n");
        }
    } /* write_packet_dumpx */

    //class NC
    //{
    //public:
    //RTSPClient *g_pRtspClient;
    //}; /* NC */

    class NCDecoder// : public NC
    {
    public:
        NCDecoder(size_t id)
            : Id(id), /*symbols(symbols), symbolSize(symbolSize),*/
              //accessedIndex(-1L),
              //pDecoderFactory(new rlnc_decoder_t::factory(g_symbols, g_symbolSize)),
              pDecoder(g_decoderFactory.build()), decodedIndex(0U),
              isOver(false), packetsReceived(0U)
  			  //decodedSymbols(std::vector<bool>(g_symbols, false))
        {
			for (int i = g_symbols - 1; i >= 0; --i)
			{
				this->decodedSymbols.push_front(i);
			}

			/*printf("sy: %d\n", this->decodedSymbols.empty() ? 1 : 0);
			for (auto x : this->decodedSymbols)
            {
				printf("%d\n", x);
			}*/
        } /* NCDecoder */

        virtual ~NCDecoder()
        {
            //delete this->pDecoder;
            //delete this->pDecoderFactory;
        } /* ~NCDecoder */

        void Decode(/*sharedPacket_t *pkt*//*unsigned char* pBuf, size_t bufSz*/ packet_t *pkt)
        {
            //write_packet_dumpx(&((*pkt)[0]), pkt->size());
            //uint8_t *symbol_id = (&((*pkt)[0])) + g_symbolSize;
            //write_packet_dumpx(symbol_id, 20);
            //printf("%0x\n", (size_t)(*symbol_id));

            //pkt->second.lock();
            //this->pDecoder->decode(&(pkt->first[0]));
            //this->pDecoder->decode(pBuf);
            //printf("decode...\n");
            this->pDecoder->decode(&((*pkt)[0]));
            //printf("decode2...\n");
            //pkt->second.unlock();

            ++this->packetsReceived;
        } /* Decode */

        bool Next(packet_t **ppPacketOut)
        {
            //printf("id: %d:= decoded index+1: %d < %d rank\n", (int)this->Id, (int)this->decodedIndex+1, (int)this->pDecoder->rank());
	        

	        // try to decode
	        //while 
            //if (this->decodedIndex + 1 < this->pDecoder->rank()/*this->symbols - 1*/)
	        //{
#ifdef IN_ORDER_DELIVERY
				if (this->decodedIndex >= g_symbols)
			    {
		            this->isOver = true;
				    return false;
			    }
				else
				{
                    if (this->pDecoder->is_symbol_decoded(this->decodedIndex))
                        //this->pDecoder->is_symbol_initialized(this->decodedIndex))
                    {
					    *ppPacketOut = new packet_t(g_symbolSize);
					    this->pDecoder->copy_symbol(this->decodedIndex,
	                        sak::storage(**ppPacketOut));
                        size_t sz = (((**ppPacketOut)[0] & 0x0f) << 8) + (**ppPacketOut)[1];
                        //unsigned char crcRead = (((unsigned char)(**ppPacketOut)[0]) & 0x70) >> 4;
                        //auto crcCalced = crc_3bit(&(**ppPacketOut)[2], sz);
                        //printf("%0x : %0x\n", crcRead, crcCalced);
                        // crc //marker valid
#if 0
					    if ( /*(((**ppPacketOut)[0] & 0xf0) == 0xf0)
                            //crcRead != crcCalced
                            // valid dequence number
                            &&*/ ( ((**ppPacketOut)[4] << 8) + (**ppPacketOut)[5] ) == 0 )
	                    {
                            write_packet_dumpx(&(**ppPacketOut)[0], (*ppPacketOut)->size());
	                        return false;
	                    }
					    else
					    {
#endif
						    ++this->decodedIndex;
                            (*ppPacketOut)->resize(sz + 2);
                            (*ppPacketOut)->erase((*ppPacketOut)->begin(), (*ppPacketOut)->begin() + 2);
                            //write_packet_dumpx(&(**ppPacketOut)[0], (*ppPacketOut)->size());
						    return true;
//					    }
                    }
				}
#else
				if (this->decodedSymbols.empty()/*this->decodedIndex + 1 >= g_symbols*/)
			    {
				    //printf("Next: Decoding is complete for %d!\n", (int)this->Id);
		            this->isOver = true;
				    return false;
			    }

            	*ppPacketOut = new packet_t(g_symbolSize);

                for (auto x : this->decodedSymbols)
                {
					//printf("%d\n", x);
                    //printf("copy..\n");
	                if (!this->pDecoder->is_symbol_decoded(x))
						// ( ((**ppPacketOut)[2] << 8) + (**ppPacketOut)[3] ) == 0 )
	                {
	                    //write_packet_dumpx(&(**ppPacketOut)[0], (*ppPacketOut)->size());
	                    //return false;
	                    continue;
	                }
	                else
	                {
			            this->pDecoder->copy_symbol(x, sak::storage(**ppPacketOut));
						size_t sz = (((**ppPacketOut)[0] & 0x0f) << 8) + (**ppPacketOut)[1];
						(*ppPacketOut)->resize(sz + 2);
                        (*ppPacketOut)->erase((*ppPacketOut)->begin(), (*ppPacketOut)->begin() + 2);
						//this->decodedSymbols[i] = true;
						//printf("decoded: %d\n", x);
						this->decodedSymbols.remove(x);
	                    return true;
	                }
                    //printf("copy2..\n");
                }
#endif
                //++this->decodedIndex;
	        //}
            //else
            //{
                return false;
            //}

            /*printf("---%d >= %d\n", *pBufSzInOut, this->symbolSize);
            assert((*pBufSzInOut) >= this->symbolSize);*/

            //std::vector<uint8_t> data_out(this->symbolSize);
            /*printf("rank: %d\n", this->pDecoder->rank());*/

            //this->pDecoder->copy_symbol(this->pDecoder->rank()/* - 1*/, sak::storage(data_out));    
        } /* Next */

        size_t Rank()
        {
            return this->pDecoder->rank();
        } /* Rank */

        float Completeness()
        {
            return this->packetsReceived / static_cast<float>(g_symbols);
        } /* Completeness */

        size_t PacketsLeft()
        {
            return g_symbols - this->packetsReceived;
        } /* PacketsLeft */

        bool IsOver()
        {
            return this->isOver;
        } /* IsOver */

        void setOver(bool over)
        {
            this->isOver = over;
        } /* setOver */

        void Feedback(unsigned char *fb)
        {
#ifdef SW_NC
            this->pDecoder->write_feedback(fb);
#endif
        } /* Feedback */

        const size_t Id;

    private:
        void reset()
        {
            //printf("New access context started for %d!\n", (int)this->Id);
            //this->pDecoder = this->pDecoderFactory->build();
	        this->decodedIndex = -1;
	        //this->accessedIndex = -1;
        } /* reset */

   		//uint32_t symbols;       // Number of symbols used for the generation
		//uint32_t symbolSize;    // The memory size of the symbols
		
		//static const rlnc_decoder_t::factory /**pD*/decoderFactory(g_symbols, g_symbolSize);   // Decoder factory
        rlnc_decoder_t::pointer pDecoder;           // Decoder instance

		long 	decodedIndex;
		//long accessedIndex;

        bool isOver;

        size_t packetsReceived;
		std::forward_list<size_t> decodedSymbols;
    }; /* NCDecoder */

    class NCRTPSource// : public NC
    {
    public:
        /* Constructor */
        NCRTPSource(/*RTSPClient *pRtspClient*/);

        /* Destructor */
        ~NCRTPSource();

        /* Adds a new symbol to the decoder. */
        void PushCode(unsigned char* pBuf, size_t bufSz);

        /* Returns a pointer to an decoded packet */
        bool PopPacket(unsigned char* pBufOut, size_t *pBufSzInOut);

#define CLEAN_UP(self, timer)                                                           \
    if ((timer % codelance::NCRTPSource::CLEAN_INTERVAL) == 0U)                         \
    {                                                                                   \
        try                                                                             \
        {                                                                               \
            /*threads.push_back(std::thread(&NCRTPSource::cleanUp, this));*/            \
            self->CleanUp();                                                            \
        }                                                                               \
        catch (std::system_error &ex)                                                   \
        {                                                                               \
            std::cout << "Exception caught when branching: " << ex.what() << std::endl; \
        }                                                                               \
        catch (...)                                                                     \
        {                                                                               \
            std::cout << "Exception caught when branching..." << std::endl;             \
        }                                                                               \
    }

        static const size_t CLEAN_INTERVAL      = 128;
        static const size_t REFRESH_INTERVAL    = 70;
        void CleanUp();

    private:
		//int next();
        //void reset(size_t id);
        NCDecoder* getDecoder(size_t id);
        NCDecoder* getDecoderIfExists(size_t id);

        void doDecode(size_t id, packet_t pkt/*unsigned char *pBuf, size_t bufSz*/);

        size_t timer;

        size_t currentId;

        //RTSPClient *pRtspClient;

		//uint32_t symbols;       // Number of symbols used for the generation
		//uint32_t symbolSize;    // The memory size of the symbols
        
        //typedef std::/*vector*/unordered_set<NCDecoder*> decoders_t;
        typedef std::list<NCDecoder*> decoders_t;        
		decoders_t decoders;

		//typedef std::pair<packet_t, bool> packet_t;
        std::mutex packetsMutex;
		packets_t packets;

        std::mutex decoderMutex;
        std::vector<std::thread> threads;

        FILE *pDump;

        //sharedPacket_t saved;
    }; /* NCRTPSource */
} /* codelance */

/* Inline definitions ------------------------------------------------------- */

inline codelance::NCRTPSource::NCRTPSource(/*RTSPClient *pRtspClient*//*uint32_t symbols, uint32_t symbolSize*/)
    :   /*pRtspClient(pRtspClient),*/ timer(0U), currentId(0U), /*symbols(symbols), symbolSize(symbolSize),*/
        //pDecoderFactory(new rlnc_decoder_t::factory(symbols, symbolSize)), 
        //pDecoder(this->pDecoderFactory->build()),
        //matId(0U), decodedIndex(-1), accessedIndex(-1)
        pDump(fopen("decoding.csv", "w"))
{
	/*for (size_t i = 0; i < g_symbols; ++i)
	{
		this->packets.push_back(packet_t(g_symbolSize));
	}*/
} /* NCRTPSource */

inline codelance::NCRTPSource::~NCRTPSource()
{
    fclose(this->pDump);
} /* ~NCRTPSource */

inline void codelance::NCRTPSource::PushCode(unsigned char *pBuf, size_t bufSz)
{
    assert(pBuf != NULL);

//#ifdef RS_CODING
//    const size_t OVERHEAD_OFFSET = 8U;
//#else
    const size_t OVERHEAD_OFFSET = 4U;
//#endif

    try
    {
        for(auto &thread : this->threads)
        {
            thread.join();
        }
        this->threads.clear();
    }
    catch (std::system_error &ex)
    {
        std::cout << "Exception caught when joining: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception caught when joining..." << std::endl;
    }

    size_t id = 0U;
    id = (pBuf[0] << 24) + (pBuf[1] << 16) + (pBuf[2] << 8) + pBuf[3];

    this->currentId = this->currentId < id ? id : this->currentId;

    //write_packet_dumpx(pBuf, bufSz);

    /* reset encoder if complete */
#if 0
    if(id != this->matId/*this->pDecoder->rank() >= this->symbols*/)
    {
        this->next();
        printf("PushCode: Decoding is complete!\n");
		this->reset(id);
    }
#endif
    //printf("PushCode:\n");
    //this->cleanUpMutex.lock();
    //auto d = this->getDecoder(id);
    // if new decoder

//printf("push1\n");
    threads.push_back(std::thread(&NCRTPSource::doDecode,
        this, id, packet_t(pBuf + OVERHEAD_OFFSET, 
        pBuf + OVERHEAD_OFFSET + (bufSz - OVERHEAD_OFFSET))/*pBuf + 4, bufSz - 4*/));
} /* PushCode */

inline void codelance::NCRTPSource::doDecode(size_t id, packet_t pkt/*unsigned char *pBuf, size_t bufSz*/)
{
    {
        std::lock_guard<std::mutex> guard(this->decoderMutex);

        //assert(pBuf);k7161
    //printf("do1\n");
        //this->saved.second.lock();
        //this->saved.first = packet_t(pBuf + 4, pBuf + (bufSz - 4));
        //this->saved.second.unlock();

        auto d = this->getDecoder(id);
        static const size_t REPEAT_COND = g_symbols - (size_t)(3.0f * g_symbols / 4.0f);
//#ifndef SW_NC
        NCDecoder *d2 = this->getDecoderIfExists(id - 1);
        if (d2 && ((d->Rank() % 10) == 0) //&& (!(d2 = this->getDecoder(id - 1))->IsOver())
            && ( (g_symbols - d2->Rank()) < REPEAT_COND ) 
            && (g_symbols - d2->Rank() > 0))
        {
            //printf("%u < %d\n", g_symbols - d2->Rank(), REPEAT_COND);
            g_pRtspClient->sendNCFeedbackCommand(id, g_symbols - d2->Rank(), 0);
        }
//#endif

    //printf("do2\n");
        d->Decode(&pkt/*pBuf, bufSz*/);

        //threads.push_back(std::thread(&codelance::NCDecoder::Decode,
        //    d, &(this->saved)));
    //printf("do3\n");
        packet_t *pPkt = NULL;
        for (auto x : this->decoders)
        {
	        while (x->Next(&pPkt))
            {
                assert(pPkt);
                /*if ( (((*pPkt)[2] << 8) + (*pPkt)[3]) == 0 )
                {
                    break;
                }*/

                this->packetsMutex.lock();
    //printf("do4\n");
                this->packets.push_back(pPkt);
                this->packetsMutex.unlock();
                //printf("push:\n");
                write_packet_dumpx(&(*pPkt)[0], pPkt->size());

                boost::posix_time::ptime now = 
                    boost::posix_time::microsec_clock::local_time();
                boost::posix_time::time_duration td = now.time_of_day();
    //printf("do5\n");
                //threads.push_back(std::thread(&fprintf, this->pDump, "%u;%u;%lld\n",
                //    x->Id, ((*pPkt)[2] << 8) + (*pPkt)[3], td.total_milliseconds()));
                /*if ( (((*pPkt)[2] << 8) + (*pPkt)[3]) != 0 )
                {*/
                    fprintf(this->pDump, "%lu;%u;%ld\n",
                        x->Id, ((*pPkt)[2] << 8) + (*pPkt)[3], td.total_milliseconds());
                    //usleep(3000);
                /*}
                else
                {
                    write_packet_dumpx(&(*pPkt)[0], pPkt->size());
                }*/

#ifdef SW_NC
                static std::vector<uint8_t> fb(100U);
                d->Feedback(&fb[0]);
                //g_pRtspClient->sendNCFeedbackCommand(id, &fb, 0);
#endif
            }
        }
    } // packets mutex

    // clean up if needed
#if 0
    try
    {
//printf("do6\n");
        //threads.push_back(std::thread(&NCRTPSource::cleanUp, this));
        this->cleanUp();
    }
    catch (std::system_error &ex)
    {
        std::cout << "Exception caught when branching: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception caught when branching..." << std::endl;
    }
#endif
    /*if (d->IsOver())
    {
        printf("Deallocating decoder %d!\n", id);
        this->decoders.erase(d*this->decoders.begin() + id*);
    }*/
} /* doDecode */

inline bool codelance::NCRTPSource::PopPacket(unsigned char* pBufOut, size_t *pBufSzInOut)
{
    std::lock_guard<std::mutex> guard(this->packetsMutex);
//printf("pop1\n");
    if (this->packets.size() == 0)
    {
        //memset()
        return false;
    }

    //printf("%lu\n", this->decoders.size());
//printf("pop2\n");
    memcpy(pBufOut, &(*this->packets.front())
        // first packet hack!!
        [0/*this->accessedIndex == 0 ? 4 : 0*/], 
		this->packets.front()->size());
//printf("pop3\n");  

    // find real end of packet
    *pBufSzInOut = this->packets.front()->size();
    /*for (size_t i = this->packets.front()->size() - 1; i >= 3U; --i)
    {
        if (pBufOut[i] == 0x02)
        {
            if (pBufOut[i - 1] + pBufOut[i - 2] + pBufOut[i - 3] == 0x00)
            {
                *pBufSzInOut = i - 3;
                break;
            }
        }
    }*/
//printf("pop4\n");
    this->packets.pop_front();
//printf("pop5\n");
	//printf("PopPacket:\n");
	//write_packet_dumpx(pBufOut, *pBufSzInOut);

    return true;

#if 0
	if (this->decodedIndex > this->accessedIndex)
	{
        ++this->accessedIndex;
		memcpy(pBufOut, &(this->packets[this->accessedIndex])
            // first packet hack!!
            [0/*this->accessedIndex == 0 ? 4 : 0*/], 
			this->packets[this->accessedIndex].size());
	    *pBufSzInOut = this->packets[this->accessedIndex].size();
		printf("PopPacket:\n");
		write_packet_dumpx(pBufOut, *pBufSzInOut);

        /* context is fully decoded */		
		if (this->accessedIndex == this->symbols - 1)
		{
			printf("PopPacket: Decoding is complete!\n");
		}
	}
	else
	{
		printf("No new decoded packets available!\n");
	}
#endif

#if 0
    printf("pop1\n");

    printf("--sym %d sz %d\n", this->symbolSize, data_out.size());
    write_packet_dumpx(&data_out[0], data_out.size());
    memcpy(pBufOut, &data_out[0], data_out.size());
    *pBufSzInOut = data_out.size();
    //write_packet_dumpx(pBufOut, data_out.size());
    printf("pop2\n");
#endif
} /* PopPacket */

inline codelance::NCDecoder* codelance::NCRTPSource::getDecoder(size_t id)
{
    static NCDecoder *pLast = nullptr;
    if (pLast && (pLast->Id == id))
    {
        return pLast;
    }
    else
    {
        for(auto d : this->decoders)
        {
            if (d->Id == id)
            {
                pLast = d;
                return d;
            }
        }
        // otherwise create new decoder
        auto p = new NCDecoder(id);
        //this->decoders.insert/*push_back*/(p);
        this->decoders.push_back(p);
        return p/*this->decoders.back()*/;
    }
} /* getDecoder */

inline codelance::NCDecoder* codelance::NCRTPSource::getDecoderIfExists(size_t id)
{
    static NCDecoder *pLast = nullptr;
    if (pLast && (pLast->Id == id))
    {
        return pLast;
    }
    else
    {
        for(auto d : this->decoders)
        {
            if (d->Id == id)
            {
                pLast = d;
                return d;
            }
        }
        return nullptr;
    }
} /* getDecoderIfExists */

inline void codelance::NCRTPSource::CleanUp()
{
//printf("cle1\n");
    std::lock_guard<std::mutex> guard(this->decoderMutex);
    //++this->timer;

    //printf("timer: %lu\n", this->timer);
    // clean up
//printf("cle2\n");
    //if ((this->timer % NCRTPSource::CLEAN_INTERVAL) == 0U)
    {
//printf("cle3\n");
        //printf("Total cleanup!\n");
        // big cleanup
        if (this->decoders.size() > g_symbols / 2)
        {
            this->decoders.remove_if
            (
                [this](NCDecoder *d) 
                {
                    assert(this);
                    return ((d->Id != this->currentId)/* && (this->decoders.size() > 1)*/);
                }
            );
//printf("e1\n");
            // only the last is left
#if 0
            size_t id = 0;
            for (auto d : this->decoders)
            {
                if (d->Id > id)
                {
//printf("e2\n");
                    id = d->Id;
                }
            }
#endif
#if 0
            for (auto d : this->decoders)
            {
                if (d->Id != this->currentId)
                {
//printf("e3\n");
                    //printf("Deallocating %scomplete decoder %lu!\n", 
                    //    d->IsOver() ? "" : "un", d->Id);
                    d->setOver(true);
                    this->decoders.erase(d);
                }
            }
#endif
        }
    }
    // refresh
// comment back
#if 0
    else if ((this->timer % NCRTPSource::REFRESH_INTERVAL) == 0U)
    {
//printf("cle4\n");
        //printf("Refresh!\n");
        for (auto d : this->decoders)
        {
            if (d->Id != this->currentId && !d->IsOver()
                && (d->PacketsLeft() < g_symbols / 3))
                //&& !d->IsOver() && d->Rank() > (2 * g_symbols / 3))
            {
                //g_pRtspClient->sendNCFeedbackCommand(d->Id, 
                //    d->PacketsLeft()/*g_symbols - d->Rank()*/, 0);
            }
        }
    }
#endif 
#if 0
    else
    {
        //printf("Cleanup!\n");
        // do clean
        size_t i = 0;
        // 80% to delete
        const float PERCENT = 0.8f;
        const size_t toDelete = static_cast<size_t>(this->decoders.size() * PERCENT);
        for(auto d : this->decoders)
        {
            if (d->IsOver())
            {
                printf("Deallocating complete decoder %lu!\n", d->Id);
                this->decoders.erase(d);
                // don't delete too much
                if (++i >= toDelete)
                {
                    break;
                }
            }
        }
    }
#endif
//printf("cle5\n");
} /* cleanUp */


#endif /* __NC_RTP_SOURCE_HH__ */

