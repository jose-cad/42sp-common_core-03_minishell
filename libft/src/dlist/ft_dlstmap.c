/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstmap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 00:12:00 by jose-cad          #+#    #+#             */
/*   Updated: 2025/09/09 00:12:00 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_dlst_append(t_dlist **head, t_dlist **last, t_dlist *new_node);

t_dlist	*ft_dlstmap(t_dlist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_dlist	*new_list;
	t_dlist	*last;
	t_dlist	*new_node;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	last = NULL;
	while (lst)
	{
		content = f(lst->content);
		new_node = ft_dlstnew(content);
		if (!new_node)
		{
			del(content);
			ft_dlstclear(&new_list, del);
			return (NULL);
		}
		ft_dlst_append(&new_list, &last, new_node);
		lst = lst->next;
	}
	return (new_list);
}

static void	ft_dlst_append(t_dlist **head, t_dlist **last, t_dlist *new_node)
{
	if (!*head)
		*head = new_node;
	else
	{
		(*last)->next = new_node;
		new_node->prev = *last;
	}
	*last = new_node;
}
