/**
 * \file
 * Embedded HTTP request/response handling framework - Session randomise
 * PROJECT: HTTP library
 * TARGET SYSTEM: Arduino, STM32, Atmel AT91SAM, portable
 */

#ifndef GHTTP_SESSIONRND_H
#define GHTTP_SESSIONRND_H

namespace ghttp {

/**
 * A class representing random session id generation
 *
 * \tparam N Session id string length, characters (including NULL)
 */
template<uint8_t N>
class SessionRnd {
public:
    /**
     * Convert an index from the acceptable session characters alphabet into ASCII
     *
     * \note Use \ref SessionRnd::AlphabetLength to get length
     * \attention We cannot use the BASE64 alphabet here as it contains "/" and "+" which are unacceptable for simple inclusion into URLs
     *
     * \param[in] i Index into session alphabet.  If larger than alphabet, character returned will be length of alphabet (integer)
     * \return ASCII character or length (integer)
     */
    static char FromAlphabet(const uint8_t i) {
        char c;

#if defined(ARDUINO_ARCH_AVR)
        PGM_VOID_P alphabet_ = T("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,-");
        static uint8_t l = PGM_STRLEN(alphabet_);

        if (i>=l) {
            c = PGM_CHAR(alphabet_+(l-1)); // Exclude NULL
        }else {
            c = PGM_CHAR(alphabet_+i);
        }
#else
        const char alphabet_[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,-";

        if (i>=sizeof(alphabet_)) {
            c = static_cast<char>(sizeof(alphabet_)-1); // Exclude NULL
        }else {
            c = alphabet_[i];
        }
#endif // !defined(ARDUINO_ARCH_AVR)

        return c;
    }


    /**
     * Get session alphabet length
     *
     * \return Length (characters)
     */
    static uint8_t AlphabetLength() {
        return static_cast<uint8_t>(FromAlphabet(255));
    }


    /**
     * Generate session id string.  Stores in internal static character array to template length N
     *
     * \attention Invokes rand and changes random seed more than once internally.  Seed will be left 
     * in a usable state, hopefully less likely related to generated session id
     *
     * \return String pointer (NULL terminated)
     */
    static const char* Generate() {
        static char scratch[N];

        // Attempt to produce some kind of non-pseudo random sequence using our 2 sources of what should/could be entrophy
        if ((rand() % 100)<50) {
            srand((GHTTP_SESSIONRND_ENTROPHY_SRC1) ^ 0xffffU ^ (GHTTP_SESSIONRND_ENTROPHY_SRC2) + (rand() % 255));
        }else {
            srand((GHTTP_SESSIONRND_ENTROPHY_SRC1) ^ 0xaaaaU ^ (GHTTP_SESSIONRND_ENTROPHY_SRC2) + (rand() % 255));
        }
        for(uint8_t i=0, e2, l=(uint8_t)(GHTTP_SESSIONRND_ENTROPHY_SRC2 ^ 0xff); i<N; i++) {
            e2 = static_cast<uint8_t>(GHTTP_SESSIONRND_ENTROPHY_SRC2);
            scratch[i] ^= static_cast<char>(e2 + (rand() % 255) ^ l);
            l = scratch[i] ^ (i&1?0x33:0xaa) + (uint8_t)(GHTTP_SESSIONRND_ENTROPHY_SRC2 ^ 0xff); 
        }

        // Align with alphabet
        for(uint8_t f,i=0, al=AlphabetLength(); i<N; i++) {
            f = 0;
            for(uint8_t u=0; u<al; u++) {
                if (scratch[i] == FromAlphabet(u)) {
                    f = 1;
                    break;
                }
            }
            if (!f) {
                // Correct
                scratch[i] = FromAlphabet(static_cast<uint8_t>(scratch[i]) % al);
            }
        }

        // This is important for the rest of the f/w which may continue taking pseudo random numbers via rand() and 
        // is then helpful to disconnect those sequences from the last generated SID
        if ((rand() % 100)<50) {
            srand((GHTTP_SESSIONRND_ENTROPHY_SRC1) ^ 0x3333U ^ (GHTTP_SESSIONRND_ENTROPHY_SRC2) + (rand() % 255));
        }else {
            srand((GHTTP_SESSIONRND_ENTROPHY_SRC1) ^ 0xffffU ^ (GHTTP_SESSIONRND_ENTROPHY_SRC2) + (rand() % 255));
        }
        rand();

        return scratch;
    }
}; // class SessionRnd

}; // namespace ghttp

#endif // GHTTP_SESSIONRND_H
