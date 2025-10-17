/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraalexa <fraalexa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:36:41 by fraalexa          #+#    #+#             */
/*   Updated: 2025/10/12 18:36:41 by fraalexa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minitalk.h"

static void	putnbr(int pid)
{
	char	c;

	if (pid >= 10)
		putnbr(pid / 10);
	c = pid % 10 + '0';
	write(1, &c, 1);
}

static void	send_ack(int signal, int cli_pid)
{
	if (kill(cli_pid, signal) == -1)
	{
		write(1, "unable to send ack", 19);
		exit (1);
	}
}

static void	sign_handler(int signal, siginfo_t *info, void *context)
{
	static int	nbits = 7;
	static char	c;

	(void)context;
	if (signal == SIGUSR1)
	{
		c |= 1 << (nbits--);
		send_ack(SIGUSR1, info->si_pid);
	}
	else if (signal == SIGUSR2)
	{
		nbits--;
		send_ack(SIGUSR1, info->si_pid);
	}
	else
		exit(1);
	if (nbits < 0)
	{
		nbits = 7;
		write(1, &c, 1);
		if (c == 0)
			send_ack(SIGUSR2, info->si_pid);
		c = 0;
	}
}

int	main(void)
{
	struct sigaction	sa;
	pid_t				pid;

	pid = getpid();
	putnbr(pid);
	write(1, "\n", 1);
	sa.sa_sigaction = &sign_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &sa, 0) == -1
		|| sigaction(SIGUSR2, &sa, 0) == -1)
	{
		write(1, "failed to launch handler functions", 34);
		exit (1);
	}
	while (1)
		pause();
	return (0);
}
