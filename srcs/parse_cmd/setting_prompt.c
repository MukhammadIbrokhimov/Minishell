/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:52:10 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/19 18:23:06 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	print_prompt()
{
	char cwd[PATH_MAX];
	char	*es;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		es = cwd + strlen(cwd);
		while (*es != '/')
			es--;
		es++;
		fprintf(stderr, "-> %s $ ", es);
	}
}


int getcmd(char *buf, int nbuf)
{
	print_prompt();
	memset(buf, 0, nbuf);
	if (fgets(buf, nbuf, stdin) == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//int
//main(void)
//{
//  static char buf[100];

//  while (getcmd(buf, sizeof(buf)) >= 0) {
//    printf("You entered: %s", buf);
//  }

//  printf("\nExiting shell.\n");
//  return 0;
//}