#include <Board/Def_config.h>
#if (PAYING_MODULE == 1)
#include <Screen/Transaction/SCR_Transaction_products.h>

LIST(Product_list);

void Productlist_clear(void){
	struct Product_t * Product = list_head(Product_list);
	struct Product_t * Product_next;
	while(Product != NULL){
		Product_next = list_item_next(Product);
		list_remove(Product_list,Product);
		if(Product->Product_string != NULL){
			free(Product->Product_string);
		}
		free(Product);
		Product = Product_next;
	}
	list_init(Product_list);
}

void Productlist_Add(int32_t price, F_TEXT(name)){
	struct Product_t * Product;

	Product = (struct Product_t *)malloc(sizeof(struct Product_t));
	//Create product
	Product->Price = price;
	Product->Product_name = name;
	Product->next = NULL;
	Product->Product_string = NULL;

	list_add(Product_list, Product);
	return;
}

void Productlist_Add_string(int32_t price,char * product_name){
	struct Product_t * Product;

	Product = (struct Product_t *)malloc(sizeof(struct Product_t));
	//Create product
	Product->Price = price;
	Product->Product_name = NULL;
	Product->next = NULL;
	Product->Product_string = malloc(strlen(product_name)+1);
	memmove(Product->Product_string,product_name,strlen(product_name)+1);

	list_add(Product_list, Product);
	return;
}


void Productlist_Handle(list_t src){
	list_copy(src, Product_list);
	return;
}

bool Productlist_total_price(int32_t *price){
	struct Product_t * Product = list_head(Product_list);
	if(Product == NULL){
		return false;
	}

	*price = 0;

	while(Product != NULL){
		*price += Product->Price;
		Product = list_item_next(Product);
	}
		return true;
}
#endif
