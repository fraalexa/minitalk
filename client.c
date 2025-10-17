/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraalexa <fraalexa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 22:43:12 by fraalexa          #+#    #+#             */
/*   Updated: 2025/10/17 11:59:25 by fraalexa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minitalk.h"

static volatile sig_atomic_t	g_ack = 0;

static int	ft_atoi(char *arr)
{
	int	i;
	int	signal;
	int	res;

	res = 0;
	signal = 1;
	i = 0;
	while (arr[i] == 32)
		i++;
	if (arr[i] == '-')
	{
		signal = -1;
		i++;
	}
	while (arr[i])
	{
		res = res * 10 + (arr[i] - '0');
		i++;
	}
	return (res * signal);
}

static void	checknsend_bit(int pid, char c, int nbit)
{
	int	cnt;

	cnt = 0;
	if ((c & (1 << nbit)) && kill(pid, SIGUSR1) == -1)
	{
		write(1, "unable to send signal", 22);
		exit(1);
	}
	else if (!(c & (1 << nbit)) && kill(pid, SIGUSR2) == -1)
	{
		write(1, "unable to send signal", 22);
		exit(1);
	}
	while (g_ack == 0 && cnt < 10000)
	{
		usleep(100);
		cnt++;
	}
	if (!g_ack)
		exit(1);
	g_ack = 0;
}

static void	send_msg(char *msg, int pid)
{
	int				nbits;
	unsigned char	byte;

	while (*msg)
	{
		nbits = 8;
		byte = (unsigned char)*msg;
		while (nbits--)
			checknsend_bit(pid, byte, nbits);
		msg++;
	}
	nbits = 8;
	while (nbits--)
		checknsend_bit(pid, '\0', nbits);
}

static void	ack_handler(int ack, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (ack == SIGUSR1)
	{
		g_ack = 1;
	}
	else if (ack == SIGUSR2)
	{
		write(1, "message sent successfully \n", 28);
	}
	return ;
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	int					pid;

	if (argc != 3)
	{
		write(1, "input error", 11);
		return (1);
	}
	pid = ft_atoi (argv[1]);
	if (pid <= 0 || kill(pid, 0) == -1)
	{
		write(2, "Invalid PID\n", 12);
		return (1);
	}
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = ack_handler;
	sigaction(SIGUSR1, &sa, 0);
	sigaction(SIGUSR2, &sa, 0);
	if (argv[2][0])
		send_msg(argv[2], pid);
	return (0);
}
