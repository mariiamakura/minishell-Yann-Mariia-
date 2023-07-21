/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycardona <ycardona@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:47:20 by ycardona          #+#    #+#             */
/*   Updated: 2023/07/21 15:29:35 by ycardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_data	*data;
	
	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (1);
	ft_parse(data);
	printf("%d\n", execve(data->tokens[0][0], data->tokens[0], NULL));
	/* int i = 0;
	int j;
	while (i <= data->pipe_num)
	{
		j = 0;
		while (data->tokens[i][j] != NULL)
		{
			printf("%sEND\n", data->tokens[i][j]);
			j++;
		}
		printf("\n");
		i++;
	} */

}

	
	//printf("num pipes: %i", data->pipe_num);
	//data->tokens[0][2] = NULL;
	
