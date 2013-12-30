#include "NCRTPSink.hh" // Current header

#include <boost/date_time/posix_time/posix_time.hpp>
//#include <time.h> 


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

namespace codelance
{
    rlnc_encoder_t::factory g_encoderFactory(g_symbols, g_symbolSize);   // Encoder factory
    //NCRTPSink *g_pNCSink = nullptr;

    NCRTPSink *NCRTPSink::pRef  = nullptr;
    size_t NCRTPSink::refCnt    = 0U;
} /* codelance */

codelance::NCEncoder::NCEncoder(size_t id)
    :   Id(id), 
        pEncoder(g_encoderFactory.build()), encodedIndex(0U)//,
        //pushedIndex(0U)//,
        //isOver(false)
#ifdef SW_NC
        , feedback(packet_t(this->pEncoder->feedback_size()))
#endif
{
} /* NCEncoder */

codelance::NCEncoder::~NCEncoder()
{
} /* ~NCEncoder */

void codelance::NCEncoder::Encode(unsigned char* pBuf, size_t bufSz)
{
    std::vector<uint8_t> data_in = std::vector<uint8_t>();
    data_in.push_back(0xf0 | ((bufSz >> 8) & 0x0f));
    data_in.push_back(bufSz & 0xff);
    for (size_t i = 0U; i < bufSz; ++i)
    {
        data_in.push_back(*(pBuf + i));
    }
    //write_packet_dumpx(&data_in[0], data_in.size());
    // push end code
    /*data_in.push_back(0x00);
    data_in.push_back(0x00);
    data_in.push_back(0x00);
    data_in.push_back(0x02);*/
    // set packet
    //printf("%d\n", this->pEncoder->rank());
    this->pEncoder->set_symbol(this->pEncoder->rank(), sak::storage(data_in));
    //write_packet_dumpx(pBuf, bufSz);
    //this->pDecoder->decode(pBuf);
} /* Encode */


uint32_t /*bool*/ codelance::NCEncoder::Next(uint8_t *pBuf)
{
    //bool isNew = false;
#ifndef SW_NC
    if (Opts::opts.systematicOff)
    {
        //printf("%u < %u\n", this->pushedIndex, g_symbols);
        if (this->pEncoder->rank() < g_symbols)
        {
            return 0;
        }
        else
        {
            // If the chosen codec stack supports systematic coding
            if(kodo::is_systematic_encoder(this->pEncoder))
            {
                if(kodo::is_systematic_on(this->pEncoder))
                {
                    kodo::set_systematic_off(this->pEncoder);
                }
            }
            if (this->encodedIndex < this->pEncoder->rank())
            {
                ++this->encodedIndex;
                //isNew = true;
            }

            return this->pEncoder->encode(pBuf);
        }
    }
    else
    {
#endif 
        if (this->encodedIndex < this->pEncoder->rank())
        {
            ++this->encodedIndex;
            //isNew = true;
        }
//printf("encode\n");
//printf("rank: %d\n", this->pEncoder->rank());
        return this->pEncoder->encode(pBuf);
#ifndef SW_NC
    }
#endif

    //return isNew;

    //return true;
#if 0
    if (this->encodedIndex + 1 >= g_symbols)
    {
        //printf("Next: Encoding is complete for %d!\n", (int)this->Id);
        this->isOver = true;
        return false;
    }

    *ppPacketOut = new packet_t(g_symbolSize);

    // ??
    // try to encode
    if (this->deodedIndex + 1 < this->pDecoder->rank()/*this->symbols - 1*/)
    {
        this->pDecoder->copy_symbol(++this->decodedIndex, 
            sak::storage(**ppPacketOut));
        return true;
    }
    else
    {
        return false;
    }

    /*printf("---%d >= %d\n", *pBufSzInOut, this->symbolSize);
    assert((*pBufSzInOut) >= this->symbolSize);*/

    //std::vector<uint8_t> data_out(this->symbolSize);
    /*printf("rank: %d\n", this->pDecoder->rank());*/

    //this->pDecoder->copy_symbol(this->pDecoder->rank()/* - 1*/, sak::storage(data_out));    
#endif
} /* Next */

#define REPEAT_BUF_SZ 50
codelance::NCRTPSink::NCRTPSink(/*uint32_t symbols, uint32_t symbolSize*/)
    :   //symbols(symbols), symbolSize(symbolSize),
        //pEncoderFactory(new rlnc_encoder_t::factory(symbols, symbolSize)), 
        //pEncoder(this->pEncoderFactory->build()),
        //, pktSz(0U)
        packets( std::deque<repeatBuffer_t>( REPEAT_BUF_SZ, repeatBuffer_t(packet_t(1500), false) ) ),
        payload(packet_t(/*this->pEncoder->payload_size() + NCRTPSink::SPEC_HDR_SZ)*/1500)),
        matId(0U), sysMatId(0U), pDump(fopen("encoding.csv", "w"))
{
    //g_pNCSink = this;
    //this->dump.open("encoding.csv");
} /* NCRTPSink */

codelance::NCRTPSink::~NCRTPSink()
{
    //delete this->pEncoderFactory;
    //this->dump.close();
    fclose(this->pDump);
} /* ~NCRTPSink */

void codelance::NCRTPSink::PushPacket(unsigned char* buf, size_t bufSz)
{
    static bool ppsDone = false;

    //printf("Push Packet: %d\n", this->encoders.size());

    auto e = this->getEncoder(this->matId);
    assert(e);


    // store SPS/PPS
    if (!ppsDone &&
        this->matId == 0U && e->Rank() < 2U)
    {
        // SPS
        if (e->Rank() == 0U)
        {
            this->sps   = packet_t(buf, buf + bufSz);
        }
        // PPS
        else
        {
            //write_packet_dumpx(buf, bufSz);
            this->pps   = packet_t(buf, buf + bufSz);
            ppsDone     = true;
        }
    }

    /* new encoder if complete */
    //printf("%u\n", e->Rank());
    if (e->Rank() >= g_symbols)
    {
        e = new NCEncoder(++this->matId);
        this->encoders./*insert*/push_back(e);
    }

    try
    {
        boost::posix_time::ptime now = 
            boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration td = now.time_of_day();
        //long stop = 16;//td.total_milliseconds();
        //clock_t time = clock() / (CLOCKS_PER_SEC / 1000);

        //this->dump << e->Id << ";";
        //this->dump << time << std::endl;
        //write_packet_dumpx(buf, bufSz);
        fprintf(this->pDump, "%u;%u;%lld\n",
            e->Id, (buf[2] << 8) + buf[3], td.total_milliseconds());
    }
    catch (std::bad_cast &ex)
    {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    } 

    e->Encode(buf, bufSz);
} /* PushPacket */

#if 0
inline void codelance::NCRTPSink::PopCode(const unsigned char* *pBufOut, size_t *pBufSzOut)
{
    *pBufOut    = &this->payload[0];
    *pBufSzOut  = this->payload.size();
} /* PopCode */
#endif

bool codelance::NCRTPSink::PopCode(OutPacketBuffer *pBuf)
{
    //printf("Pop Code\n");
    NCEncoder *e = nullptr;

    if (Opts::opts.systematicOff)
    {
        while ( !(e = this->getEncoderIfExists(this->sysMatId)) ||  (this->sysMatId < this->matId) )
        {
            //printf("%d\n", this->sysMatId);
            this->sysMatId = this->sysMatId < this->matId ? this->sysMatId + 1 : this->matId;
        }
    }
    else
    {
        e = this->getEncoder(this->matId);
    }
    assert(e);

    //std::fill(this->payload.begin(), this->payload.end(), 0U);
    /*auto sz = */
    /*bool isNew*/
    if ( e->Next(&(this->payload[4])) == 0 )
    {
        return false;
    }
    //printf("--sz=%lu\n");
    //this->pEncoder->encode(&(this->payload[4]));

    // id
    this->payload[0/*4*/] = (this->matId >> 24) & 0xff;
    this->payload[1/*5*/] = (this->matId >> 16) & 0xff;
    this->payload[2/*6*/] = (this->matId >> 8) & 0xff;
    this->payload[3/*7*/] = this->matId & 0xff;

    // get size
    /*int i = 0;
    for (i = this->payload.size() - 1; i >= 0; --i)
    {
        if (this->payload[i] != 0x00)
        {
            break;
        }
    }*/
    //write_packet_dumpx(&(this->payload[0]), e->Size()+4);
    pBuf->enqueue(&(this->payload[0]), /*i + 1*/e->Size()+4/*this->payload.size()*/);

    /*this->payload[1] = (this->matId >> 16) & 0xff;
    this->payload[2] = (this->matId >> 8) & 0xff;
    this->payload[3] = this->matId & 0xff;*/

    this->maintain();

    return true;
} /* PopCode */

void/*std::vector<codelance::packet_t>*/ codelance::NCRTPSink::Repeat(size_t times, long id)
{
    //printf("Repeat\n");

    if (times > (g_symbols / 2.0f))
    {
        return;
    }

    // current id
    if (id < 0)
    {
        id = this->matId;
    }


    //std::vector<packet_t> p;

    typedef std::pair<size_t, size_t> buf_t;
    static std::deque<buf_t> buffer;
    buffer.push_back(buf_t(times, id));
    while (buffer.size() > 0)
    {
        times   = buffer.front().first;
        id      = buffer.front().second;

        auto e = this->getEncoderIfExists(id);
        if (e)
        {
            if (Opts::opts.repeatAfterOver && !e->IsOver())
            {
                return;
            }

            //printf("%d - %d\n", id, times);
            for(size_t i = 0U; i < times; ++i)
            {
                // get free packet
                repeatBuffer_t *pRb = nullptr;
                for (size_t i = 0; i < this->packets.size(); ++i)
                {
                    if (this->packets[i].second == false)
                    {
                        pRb = &this->packets[i];
                        break;
                    }
                }

                if (pRb)
                {
                    //packet_t pkt(e->Size()/*1500*/);
                    packet_t *pkt = &pRb->first;
    //printf("before\n");
                    e->Next(&((*pkt)[/*5*/4]));

                    // id
                    (*pkt)[0] = (id >> 24) & 0xff;
                    (*pkt)[1] = (id >> 16) & 0xff;
                    (*pkt)[2] = (id >> 8) & 0xff;
                    (*pkt)[3] = id & 0xff;
                    // non-systematic
                    //(*pkt)[4] = 0xff;

                    pkt->resize(e->Size());
                    //this->packets.push_back(pkt);
                    pRb->second = true;
                } // if pRb
                else
                {
                    printf("Repeat buffer is full!\n");
                }
            } // for times
            //printf("Encoder %ld repeated %u times!\n", id, times);
        } // if e
        else
        {
            printf("Encoder %ld is already removed!\n", id);
            //p.clear();
        }
        buffer.pop_front();
    } // while true
    //printf("asdsss");
    //return p;
} /* Repeat */

void codelance::NCRTPSink::Feedback(long id, packet_t *fb)
{
    auto e = this->getEncoderIfExists(id);
    if (e)
    {
        e->Feedback(fb);
    }
    else
    {
        printf("Encoder %ld is already removed!*\n", id);
    }
} /* Feedback */

codelance::NCEncoder* codelance::NCRTPSink::getEncoder(size_t id)
{
    //printf("getEncoder\n");

    NCEncoder* e = nullptr;
    for (auto e : this->encoders)
    {
        if (e->Id == id)
        {
            //e = c;
            return e;
        }
    }

    /* new encoder */
    e = new NCEncoder(/*++*/this->matId);
    this->encoders./*insert*/push_back(e);

    return e;
} /* getDecoder */

codelance::NCEncoder* codelance::NCRTPSink::getEncoderIfExists(size_t id)
{
    //printf("getEncoderIfExists\n");

    for (auto e : this->encoders)
    {
        if (e->Id == id)
        {
            return e;
        }
    }

    return nullptr;
} /* getDecoder */

