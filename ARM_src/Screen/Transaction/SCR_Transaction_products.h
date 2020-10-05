#include <Board/Def_config.h>
#if (PAYING_MODULE == 1)

#ifndef __TRANSACTION_PRODUCTS_H__
#define __TRANSACTION_PRODUCTS_H__

#include <Core/Memory/list.h>
#include <Core/Util/U_string.h>
#include <Device/Net/CCV/CCV_global.h>
#include <stdlib.h>
#include <Screen/SCR_Text.h>

struct Product_t{
	struct Product_t * next;
	int32_t Price;
	F_TEXT(Product_name);
	char * Product_string;
};

/*
 * @brief Add new product to product list. Clear list after use
 * @param price price of the product
 * @param name name deceleration of the product
 * @return void
 */
void Productlist_Add(int32_t price, F_TEXT(name));

/*
 * @brief Add new product to product list. Clear list after use
 * @param price price of the product
 * @param name name string of the product
 * @return void
 */
void Productlist_Add_string(int32_t price,char * product_name);

/*
 *@brief Clear product list
 */
void Productlist_clear(void);

/*
 *@brief Copy handle to product list
 */
void Productlist_Handle(list_t src);

/*
 * @brief get total price of product list when no product list is created the original price is returnd;
 * @param price pointer to price
 * @return true when price is from product list. false when no list is found
 */
bool Productlist_total_price(int32_t *price);

#endif //__TRANSACTION_PRODUCTS_H__
#endif
