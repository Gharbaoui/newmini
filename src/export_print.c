/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:17 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:18 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sort_words(t_words **words)
{
	t_words	*head;

	head = *words;
	real_work_sort(&head);
	*words = head;
	return (SUCCESS);
}

void	real_work_sort(t_words **words)
{
	t_words	*head;
	t_words	*a;
	t_words	*b;

	head = *words;
	a = NULL;
	b = NULL;
	if (head == NULL || head->next == NULL)
		return ;
	split_words(head, &a, &b);
	real_work_sort(&a);
	real_work_sort(&b);
	*words = sortd_merg(a, b);
	free_words(&head);
}

void	split_words(t_words *words, t_words **a, t_words **b)
{
	int	half;
	int	i;

	i = -1;
	half = countnumberofcmds(words) / 2;
	while (++i < half)
	{
		mk_and_add_to_words(a, words->txt);
		words = words->next;
	}
	while (words)
	{
		mk_and_add_to_words(b, words->txt);
		words = words->next;
	}
}

t_words	*sortd_merg(t_words *w1, t_words *w2)
{
	t_words	*h1;
	t_words	*h2;
	t_words	*fin;

	if (w1 == NULL)
		return (w2);
	if (w2 == NULL)
		return (w1);
	fin = NULL;
	h1 = w1;
	h2 = w2;
	while (w1)
		if (help_sortd_merg(&w1, &w2, &fin))
			break ;
	if (w1)
		fill_rest(&fin, w1);
	else if (w2)
		fill_rest(&fin, w2);
	free_words(&h1);
	free_words(&h2);
	return (fin);
}

void	fill_rest(t_words **fin, t_words *w)
{
	while (w)
	{
		mk_and_add_to_words(fin, w->txt);
		w = w->next;
	}
}
