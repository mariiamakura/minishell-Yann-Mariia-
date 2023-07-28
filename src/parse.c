/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycardona <ycardona@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:27:39 by ycardona          #+#    #+#             */
/*   Updated: 2023/07/28 17:54:47 by ycardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_quotation(char *str, int *i, int quot)
{
	int j;

	if (str[*i] == quot) //skipp the pipes in " "
	{
		j = 1;
		while (str[*i + j])
		{
			if (str[*i + j] == quot)
			{
				*i += j;
				break;
			}
			j++;
		}
	}
}

int		ft_count_pipes(char *input) //counts number of pipes
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (input[0] == '|')
		exit (-1); //add the correct error message
	while (input[i])
	{
		ft_quotation(input, &i, 34); //skipps " "
		ft_quotation(input, &i, 39); //skipps ' '
		if (input[i] == '|')
			count++;
		i++;
	}
	return (count);
}

void	ft_skipp_redir(char *str, int *i, int *count)
{
	if (str[*i] == '>' || str[*i] == '<')
	{
		if (0 < *i && str[*i - 1] != ' ')
			*count += 1;
		if ((str[*i] == '>' && str[*i + 1] == '>')
			|| (str[*i] == '<' && str[*i + 1] == '<')) //skipps one < if <<
			*i += 1;
		if (str[*i + 1] == ' ')
		{
			*i += 1;
			while (str[*i] == ' ')
				*i += 1;
			if (!str[*i])
				*i -= 1;
		}
	}
}

static int	ft_count_args(char *s) //counts number of arguments
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i])
		{
			while (s[i] != ' ' && s[i])
			{
				ft_quotation(s, &i, 34); //skipps " "
				ft_quotation(s, &i, 39); //skipps ' '
				ft_skipp_redir(s, &i, &count);
				i++;
			}
			count++;
			i--;
		}
		i++;
	}
	return (count);
}

char	*ft_substr_pipe(char *str, unsigned long *start) //returns the substring before the pipe as string 
{
	char	*sub;
	int		i;
	
	i = *start;
	/* if (!(str + i))
		return (NULL); */
	while (str[i] && str[i] != '|')
	{
		ft_quotation(str, &i, 34); //skipps " "
		ft_quotation(str, &i, 39); //skipps ' '
		i++;
	}
	sub = (char *) malloc(i - *start + 1);
	if (sub == NULL)
		return (NULL);
	ft_memmove(sub, str + *start, i - *start + 1);
	sub[i - *start] = '\0';
	*start = i + 1;
	return (sub);
}

int	ft_quotations_count(char *str, int quot)
{
	int j;
	
	j = 0;
	if (str[j] == quot) //handling " "
	{
		j++;
		while (str[j])
		{
			if (str[j] == quot)
				return (j);
			j++;
			if(str[j] == '\0')
				return (0);
		}
	}
	return (0);
}

int	ft_split_sub(char *sub_str, int block, t_data *data)
{
	int i;
	int	j;
	int	arg;

	i = 0;
	arg = 0;
	while(sub_str[i])
	{
		if (sub_str[i] != ' ')
		{
			j = 0;
			while (sub_str[i + j] != ' ' && sub_str[i + j])
			{
				if (sub_str[i + j] == '<' || sub_str[i + j] == '>') // < und > trennen ---> getrennte funktion 
				{
					j++;
					if (sub_str[i + j] == '<' || sub_str[i + j] == '>')
						j++;
					while (sub_str[i + j] == ' ')
						j++;
				}
				j += ft_quotations_count(&sub_str[i + j], 34); // skipp " "
				j += ft_quotations_count(&sub_str[i + j], 39); // skipp ' '
				j++;
				if (sub_str[i + j] == '<' || sub_str[i + j] == '>')
					break ;
			}
			data->tokens[block][arg] = ft_calloc(sizeof(char), j + 1);
			if (data->tokens[block][arg] == NULL)
				exit (1);
			ft_memmove(data->tokens[block][arg], sub_str + i, j + 1);
			data->tokens[block][arg][j] = '\0';
			arg++;
			i += j - 1;
		}
		i++;
	}
	return (0);

}

int	ft_parse(t_data *data)
{
	char	*input;
	int		i;
	unsigned long start;
	char	*sub_str;
	int		argc;

	input = readline(" ~ minishell$ ");
	add_history(input);
	if (input == NULL) //handles ctrl+d
	{
		write(1, "exit\n", 6);
		exit(-1);
	}
	if (*input == '\0')
		return (free(input), -1);
	data->pipe_num = ft_count_pipes(input);
	data->tokens = ft_calloc(sizeof(char **), (data->pipe_num + 1)); //no NULL termination because we know pipe_num
	if (data->tokens == NULL)
		exit (1);
 	i = 0;
	start = 0;
	while (i <= data->pipe_num)
	{
		sub_str = ft_substr_pipe(input, &start); //get substring untill pipe
		argc = ft_count_args(sub_str); //count arguments in substring
		data->tokens[i] = ft_calloc(sizeof(char *), argc + 1);
		if (data->tokens[i] == NULL)
			exit (1);
		ft_split_sub(sub_str, i, data); //split and write the substring into tokens
		data->tokens[i][argc] = NULL;//NULL-termination required for execve
		free(sub_str);
		i++;
	}
	free(input);
	return (0);
}
