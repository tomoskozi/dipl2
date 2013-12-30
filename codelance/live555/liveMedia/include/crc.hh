#ifndef CRC_H
#define CRC_H

typedef unsigned        uint;
typedef unsigned char   uchar;

/**
 *  Calculate the 3-bit CRC using the polynomial 1 + x + x^3 for the given
 *  data and data length.
 *  The initial value 0x07 must be set outside this function. The resulting
 *  CRC value is stored in the lower 3 bits of *crc.
 *  @param data [in] Pointer to the start of data for CRC calculation.
 *  @param len [in] Length of the data for CRC calculation.
 *  @param crc [in,out] Pointer to the initial CRC value. This value is updated
 *                      during the CRC calculation.
 */
uint crc_3bit(const uchar* const data, const uint len);

/**
 *  Calculate the 7-bit CRC using the polynomial 1 + x + x^2 + x^3 + x^6 + x^7
 *  for the given data and data length.
 *  The initial value 0x7F must be set outside this function. The resulting
 *  CRC value is stored in the lower 7 bits of *crc.
 *  @param data [in] Pointer to the start of data for CRC calculation.
 *  @param len [in] Length of the data for CRC calculation.
 *  @param crc [in,out] Pointer to the initial CRC value. This value is updated
 *                      during the CRC calculation.
 */
uint crc_7bit(const uchar* const data, const uint len);

/**
 *  Calculate the 8-bit CRC using the polynomial 1 + x + x^2 + x^8
 *  for the given data and data length.
 *  The initial value 0xFF must be set outside this function. The resulting
 *  CRC value is stored in *crc.
 *  @param data [in] Pointer to the start of data for CRC calculation.
 *  @param len [in] Length of the data for CRC calculation.
 *  @param crc [in,out] Pointer to the initial CRC value. This value is updated
 *                      during the CRC calculation.
 */
uint crc_8bit(const uchar* const data, const uint len);

/**
 *  Calculate the 32-bit CRC using the polynomial 1 + x + x^2 + x^4 + x^5
 *  + x^7 + x^8 + x^10 + x^11 + x^12 + x^16 + x^22 + x^23 + x^26 + x^32
 *  for the given data and data length.
 *  The initial value 0xFFFFFFFF must be set outside this function. The
 *  resulting CRC value is stored in *crc.
 *  @param p_data [in] Pointer to the start of data for CRC calculation
 *  @param len [in] Length of the data for CRC calculation
 *  @param crc [in,out] Pointer to the initial CRC value. This value is updated
 *                      during the CRC calculation.
 */
uint crc_32bit(const uchar* const data, const uint len);


#endif /* CRC_H */

