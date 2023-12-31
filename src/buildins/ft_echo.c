/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparasku <mparasku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 18:40:47 by mparasku          #+#    #+#             */
/*   Updated: 2023/08/28 14:58:59 by mparasku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo(char *av[], t_data *data, int index)
{
	if (av[1] == NULL)
		ft_putstr_fd("\n", data->pipes[index][1]);
	else if (ft_all_n(av[1]) == TRUE)
	{
		ft_print(av, TRUE, index, data);
	}
	else 
		ft_print(av, FALSE, index, data);
	return (0);
}

int	ft_all_n(char *option)
{
	int	i;

	i = 0;
	if (option[i] != '-')
		return (FALSE);
	i++;
	while (option[i])
	{
		if (option[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	ft_print(char *av[], int flag, int index, t_data *data)
{
	char	*home_dir;
	int		i;

	home_dir = ft_get_env_value(data, "HOME");
	i = 1;
	if (flag == TRUE)
		i = 2;
	while (av[i])
	{
		if ((ft_strncmp(av[i], "~", ft_strlen(av[i]))) == 0 
			&& ft_strlen(av[i]) == ft_strlen("~"))
		{
			ft_putstr_fd(home_dir, data->pipes[index][1]);
		}
		else 
		{
			ft_putstr_fd(av[i], data->pipes[index][1]);
		}
		if (av[i + 1] != NULL)
			ft_putstr_fd(" ", data->pipes[index][1]);
		i++;
	}
	if (flag == FALSE)
		ft_putstr_fd("\n", data->pipes[index][1]);
	free(home_dir);
}
