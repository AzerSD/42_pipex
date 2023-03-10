/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_2d_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asioud <asioud@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 13:12:06 by asioud            #+#    #+#             */
/*   Updated: 2022/12/20 13:12:07 by asioud           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	print_2d_fd(char **arr, int32_t fd)
{
	size_t	i;

	i = 0;
	while (arr[i])
		ft_printf_fd(fd, "%s\n", arr[i++]);
}
