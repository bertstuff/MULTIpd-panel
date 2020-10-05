#ifndef __ACCESS_CODE_H__
#define __ACCESS_CODE_H__

#include <stdint.h>

/*
 * @brief save secret code to sd file
 */
void Save_code_to_sd(uint16_t auth_lev, uint32_t code);

/*
 * @brief load secret code from sd card
 */
uint32_t Load_code_from_sd(uint16_t auth_lev);

/*
 *  @brief search for code on sd
 *  @code code to find
 *  @return true when found
 */
bool Access_Code_present_on_sd(uint8_t Code[7],uint8_t Code_lenght);
#endif //__ACCESS_CODE_H__
