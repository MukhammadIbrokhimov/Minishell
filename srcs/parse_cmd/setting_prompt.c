/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:52:10 by muxammad          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/19 18:23:06 by muxammad         ###   ########.fr       */
=======
/*   Updated: 2025/04/21 13:00:40 by muxammad         ###   ########.fr       */
>>>>>>> feat/prompt
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

<<<<<<< HEAD

int getcmd(char *buf, int nbuf)
{
	print_prompt();
	memset(buf, 0, nbuf);
	if (fgets(buf, nbuf, stdin) == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
=======
char *getcmd(void)
{
	char cwd[PATH_MAX];
	char prompt[PATH_MAX];
	char *cmd;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		char *es = strrchr(cwd, '/');
		if (es)
			snprintf(prompt, sizeof(prompt), "-> %s $ ", es + 1);
		else
			snprintf(prompt, sizeof(prompt), "-> ? $ ");
	}
	else
		snprintf(prompt, sizeof(prompt), "-> ? $ ");

	cmd = readline(prompt);

	if (cmd && *cmd)
		add_history(cmd);

	return (cmd);  // caller must free(cmd)
>>>>>>> feat/prompt
}

//int
//main(void)
//{
<<<<<<< HEAD
//  static char buf[100];

//  while (getcmd(buf, sizeof(buf)) >= 0) {
//    printf("You entered: %s", buf);
=======
	
//  while (1) {
//	char *cmd = getcmd();
//    printf("You entered: %s", cmd);
>>>>>>> feat/prompt
//  }

//  printf("\nExiting shell.\n");
//  return 0;
//}