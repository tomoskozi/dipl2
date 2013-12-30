#ifndef __NC_RTP_SINK_HH__
#define __NC_RTP_SINK_HH__

#define SW_NC//SW_NC//RS//FULL_NC//SW_NC
#include <kodo/rlnc/full_vector_codes.hpp>
#include <kodo/rlnc/on_the_fly_codes.hpp>
#include <kodo/rs/reed_solomon_codes.hpp>
#ifdef SW_NC
#include <kodo/rlnc/sliding_window_encoder.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>
#endif

#include <deque>
#include <unordered_set>
#include <algorithm>
//#include <fstream>
#include <cstdio>

#include "RTPSink.hh"

#include "opts.hh"


namespace codelance
{
    typedef std::vector<uint8_t> packet_t;
    typedef std::pair<packet_t, bool> repeatBuffer_t;

    const uint32_t g_symbols    = 32;//16;//8;//32;
    const uint32_t g_symbolSize = 1300;
    const size_t MAX_ENCODERS   = 10U;//10U;
//#define PERIODICAL_DROPS
#ifdef ONLINE_NC
    typedef kodo::on_the_fly_encoder<fifi::binary8> rlnc_encoder_t;
#endif
#ifdef FULL_NC
    typedef kodo::full_rlnc_encoder<fifi::binary8> rlnc_encoder_t;
#endif
#ifdef RS
    typedef kodo::rs_encoder<fifi::binary8> rlnc_encoder_t;
#endif
#ifdef SW_NC
    typedef kodo::sliding_window_encoder<fifi::binary8> rlnc_encoder_t;
#endif

    class NCEncoder
    {
    public:
        NCEncoder(size_t id);

        virtual ~NCEncoder();

        void Encode(unsigned char* pBuf, size_t bufSz);

        
        uint32_t /*bool*/ Next(uint8_t *pBuf);

        bool IsOver()
        {
            //return this->isOver;
            //printf("%d >= %d\n", this->pEncoder->rank(), g_symbols);
            return this->pEncoder->rank() >= g_symbols;
        } /* IsOver */

        size_t Size()
        {
            return this->pEncoder->payload_size();
        } /* Size */

        size_t Rank()
        {
            return this->pEncoder->rank();
        } /* Rank */

        bool IsNextNew()
        {
            return this->encodedIndex >= this->Rank() ? false : true;
        } /* IsNextNew */

        void Feedback(packet_t *fb)
        {
#ifdef SW_NC
            this->pEncoder->read_feedback(&(*fb)[0]);
#endif
        } /* Feedback */

        const size_t Id;

    private:
#if 0
        void reset()
        {
	        this->encodedIndex = -1;
        } /* reset */
#endif
		
        rlnc_encoder_t::pointer pEncoder;   // Decoder instance

		size_t encodedIndex;
        //size_t pushedIndex;

        //bool isOver;

#ifdef SW_NC
        packet_t feedback;
#endif
    }; /* NCEncoder */

    /* Singleton */
    class NCRTPSink
    {
    public:
        static NCRTPSink* Create()
        {
            if (NCRTPSink::refCnt == 0U)
            {
                NCRTPSink::pRef = new NCRTPSink;
            }
            ++NCRTPSink::refCnt;
            return NCRTPSink::pRef;
        } /* Create */

        static void Release()
        {
            if (--NCRTPSink::refCnt == 0U)
            {
                delete NCRTPSink::pRef;
                NCRTPSink::pRef = nullptr;
            }
        } /* Release */

        /* Adds a new symbol to the encoder. */
        void PushPacket(unsigned char* buf, size_t bufSz);

        /* Returns a pointer to an encoded packet */
        //void PopCode(const unsigned char* *pBufOut, size_t *pBufSzOut);
        bool PopCode(OutPacketBuffer *pBuf);

        bool IsComplete()
        {
            //return this->pEncoder->rank() >= this->symbols;
            assert(this->getEncoder(this->matId));
            return this->getEncoder(this->matId)->IsOver();
        } /* IsComplete */

        bool IsNextNew()
        {
            auto e = this->getEncoder(this->matId);
            assert(e);

            return e->IsNextNew();
        } /* IsNextNew */

        /*std::vector<packet_t>*/ void Repeat(size_t times, long id = -1);

        //std::deque<packet_t> packets;
        std::deque<repeatBuffer_t> packets;

        void SendParameters()
        {
            //printf("Sending parameters!\n");
            this->PushPacket(&(this->sps[0]), this->sps.size());
            this->PushPacket(&(this->pps[0]), this->pps.size());
        } // SendParameters    

        void Feedback(long id, packet_t *fb);

    protected:
    /* Constructor */
    NCRTPSink();

    /* Destructor */
    ~NCRTPSink();

    private:
        void maintain()
        {
            //printf("pRef: %d\n", this->refCnt);

            while (this->encoders.size() > MAX_ENCODERS)
            {
                //printf("Deallocating %d!\n", this->encoders.front()->Id);
                this->encoders.pop_front();
            }
        } /* maintain */

        static NCRTPSink *pRef;
        static size_t refCnt;

        //uint32_t symbols;       // Number of symbols used for the generation
        //uint32_t symbolSize;    // The memory size of the symbols
        NCEncoder* getEncoder(size_t id);
        NCEncoder* getEncoderIfExists(size_t id);
        
        //typedef kodo::on_the_fly_encoder<fifi::binary8> rlnc_encoder_t;
        //rlnc_encoder_t::factory *pEncoderFactory;   // Encoder factory
        //rlnc_encoder_t::pointer pEncoder;           // Encoder instance
    
        //typedef std::vector<uint8_t> payload_t;
        packet_t payload;                          // For coded packets

        static const size_t SPEC_HDR_SZ = 4;
        static const size_t END_CODE_SZ = 4;

        size_t matId;
        size_t sysMatId;
        //std::deque<size_t> pktSz;

        typedef std::deque<NCEncoder*> encoders_t;
		encoders_t encoders;

        packet_t sps;
        packet_t pps;

        //std::ofstream dump;
        FILE *pDump;
    }; /* NCRTPSink */
} /* codelance */

/* Inline definitions ------------------------------------------------------- */


#endif /* __NC_RTP_SINK_HH__ */

