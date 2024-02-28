
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_product{
	char *product_name;
	double price;
	struct s_product *next;
} t_product;

typedef struct s_data{
	char *city;
	double total;
	t_product *head_products;
	struct s_data *next;
}	t_data;

typedef struct s_alpha{
	char alpha;
	t_data *head_cities;
}	dictionary;

typedef struct s_city{
	char alpha;
	t_data *result;
} final_cities;

t_product *create_product(char *name, double prix);
void delete_last_node(t_product **products);
t_data *create_city(char **splitted);
int check_ifexist(t_product *products, char **product);
void add_if_cheap(t_product **products, char **splited);
void check_exist_or_add(t_data **cities, char **splited, final_cities *result);
void free_all(dictionary *alphabet);

#endif
