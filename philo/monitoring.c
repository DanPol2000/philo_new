/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorse <chorse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 14:21:35 by chorse            #+#    #+#             */
/*   Updated: 2022/04/30 17:53:56 by chorse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	time_of_finish(t_philo *philo)
{
	long	time;

	pthread_mutex_lock(philo->time);
	time = ft_time();
	if (time - philo->start_eat > philo->time_die)
	{
		if (philo->time_start == 0 && time % 1000 < philo->time_die)
		{
			pthread_mutex_unlock(philo->time);
			return (0);
		}
		pthread_mutex_unlock(philo->time);
		return (time);
	}
	pthread_mutex_unlock(philo->time);
	return (0);
}

void	kill_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number)
	{
		pthread_detach(data->thread[i]);
		i++;
	}
}

void	destroy_mt(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(data->print);
	pthread_mutex_destroy(data->time);
	pthread_mutex_destroy(data->num_times);
	free(data->philo);
	free(data->forks);
	free(data->time);
	free(data->print);
	free(data->thread);
	free(data->num_times);
}

void	*moni(void *arg)
{
	int		i;
	long	time;
	t_data	*data;

	i = 0;
	data = arg;
	while (1)
	{
		if (check_time(data) == 0)
			ft_sleep(100);
		time = time_of_finish(&data->philo[i]);
		if (time > 0)
		{
			red_button(data, i, time);
			break ;
		}
		if (ft_circle(data, i) == 0)
			break ;
		i++;
		if (i == data->number)
			i = 0;
	}
	return (NULL);
}

void	red_button(t_data *data, int i, long time)
{
	pthread_mutex_lock(data->print);
	printf("%lld %d died\n", \
		time - data->philo[i].time_start, data->philo[i].id);
	kill_philo(data);
	destroy_mt(data);
}
