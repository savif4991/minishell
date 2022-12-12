#include "../minishell.h"

/**
 * @brief 
 * 빌트인 unset 함수입니다. 옵션을 지원하지 않습니다.
 * original : unset [-fv] name...
 * @return int 성공 시 0, 실패 시 -1
 */
int	ft_unset(char *name, t_env *env)
{
	if (delete_env(env, name))
		return (-1);
	return (0);
}