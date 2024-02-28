#include "header.h"

int strlen_lst(t_product *products)
{
	int i = 0;
	
	while (products)
	{
		products = products->next;
		i++;
	}
	return (i);
}

void delete_last_node(t_product **products)
{
	t_product *cpy = *products;
	while (cpy->next->next)
		cpy = cpy->next;
	free(cpy->next);
	cpy->next = NULL;
}

t_data *create_city(char **splitted)
{
	t_data *newc = (t_data *) malloc (sizeof(t_data));
	if (newc)
	{
		newc->city = strdup(*splitted);
		newc->total = atof(*(splitted + 2));
		newc->next = NULL;
		newc->head_products = create_product(*(splitted + 1), newc->total);
		return (newc);
	}
	else
		return 0;
}

t_product *create_product(char *name, double prix)
{
	t_product *newp = (t_product *) malloc (sizeof(t_product));
	if (newp)
	{
		newp->product_name = strdup(name);
		newp->price = prix;
		newp->next = NULL;
		return (newp);
	}
	else
		return 0;
}

int check_ifexist(t_product *producta, char **products)
{
	t_product *cpy = producta;
	while (cpy)
	{
		if (!strcmp(cpy->product_name, *(products + 1)))
			return (1);
		cpy = cpy->next;
	}
	return (0);
}

void read_cheap(t_product *pds)
{
	printf("----------------products-----------------\n");
	while (pds)
	{
		printf("(%s:%f)\n", pds->product_name, pds->price);
		pds = pds->next;
	}
	printf("\n");
}

void add_if_cheap(t_product **products, char **splited)
{
	t_product *cpy = *products;
	double fprice = atof(*(splited + 2));
	
	if (check_ifexist(cpy, splited))
	{
		while (cpy)
		{
			if (!strcmp(cpy->product_name, *(splited + 1)))
			{
				if (cpy->price > fprice)
					cpy->price = fprice;
				break;
			}
		}
	}
	else
	{
		t_product *newp = create_product(*(splited + 1), fprice);
		
		if (cpy->price > fprice)
		{
			newp->next = cpy;
		}
		else
		{
			while (cpy->next)
			{
				if (cpy->next->price > fprice)
				{
					newp->next = cpy->next;
					cpy->next = newp;
					break;
				}
				cpy = cpy->next;
			}
			if (strlen_lst(*products) < 5 && !cpy->next)
				cpy->next = newp;

		}
		if (strlen_lst(*products) > 5)
			delete_last_node(products);
	}
}

void	read_lst_cities(t_data *citiess)
{
	printf("---------cities ---------------\n");
	while (citiess)
	{
		read_cheap(citiess);
		citiess = citiess->next;
	}
		printf("\n");
}


void check_exist_or_add(t_data **citiess, char **splited, final_cities *resulta)
{
	char **cpy_split = splited;
	t_data *min;
	if (resulta)
		min = resulta->result;

	if (*citiess)
	{
		t_data *cpy = *citiess;
		t_data *saver = cpy;
		int found = 0;
		
		while (cpy)
		{
			if (!strcmp(*splited, cpy->city))
			{
				found = 1;
				cpy->total += atof(*(splited + 2));
				add_if_cheap(&cpy->head_products, splited);
			}
			if ((min && min->total < cpy->total) || !min)
				min = cpy;
			if (!(cpy->next))
				saver = cpy;
			cpy = cpy->next;

		}
		if(!found)
		{
			t_data *mdina = create_city(splited);
			saver->next = mdina;
			if ((min && min->total > mdina->total) || !min)
				min = mdina;
		}

	}
	else
	{
		t_data *mdina = create_city(splited);
		*citiess = mdina;
		min = mdina;
	}
	resulta->result = min;
	resulta->alpha = **splited;
}

void free_all(dictionary **alphabet)
{
	t_data *cpy;
	
	if (alphabet && *alphabet)
	{
			while ((*alphabet)->head_cities->head_products)
			{
				printf("here\n");
				cpy = (*alphabet)->head_cities;
				delete_last_node(&cpy);

			}
			while ((*alphabet)->head_cities)
			{
				cpy = (*alphabet)->head_cities->next;
				free((*alphabet)->head_cities);
				(*alphabet)->head_cities = cpy;
			}
	}
	else
		printf("NOT FOUnd \n");
}

static int	get_words(char const *s, char c, int *index)
{
	int		counter;
	int		found;

	counter = 0;
	while (*s)
	{
		found = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			s++;
			found = 1;
		}
		counter += found;
	}
	*index = counter;
	return (counter + 1);
}

static int	ft_strsdup(char **array, const char *s, int len)
{
	int		i;
	char	*arr;

	i = -1;
	arr = (char *) malloc (sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	while (++i < len && s[i])
		arr[i] = s[i];
	arr[i] = 0;
	*array = arr;
	return (1);
}

char	**free_arr(char ***array, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free((*array)[i++]);
	free(*array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		i;
	int		len;
	int		words;

	i = -1;
	words = 0;
	array = (char **) malloc (sizeof(char *) * get_words(s, c, &words));
	if (!array)
		return (NULL);
	while (++i < words)
	{
		len = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			len++;
			s++;
		}
		if (!ft_strsdup(&array[i], s - len, len))
			return (free_arr(&array, i));
	}
	array[words] = 0;
	return (array);
}


int main()
{
	FILE *f = fopen("input.txt", "r");
	char ligne[100];
	dictionary alphabet[26] = {NULL};
	char **splited;
	final_cities resultat[26];
	
	while (fgets(ligne, 100, f))
	{
		if ((splited = ft_split(ligne, ',')))
		{
			alphabet[**splited - 'A'].alpha = **splited;
			check_exist_or_add(&alphabet[**splited - 'A'].head_cities, splited, &resultat[**splited - 'A']);
			free(*splited);
			free(*(splited + 1));
			free(*(splited + 2));
			read_lst_cities(alphabet[**splited - 'A'].head_cities);
		}
		else
			break;
	}
	int i = 0;
	while (i < 26)
	{
		printf("here\n");
			resultat[i].result = NULL;
		free_all(&alphabet[i]);
		i++;
	}
	fclose(f);
}

