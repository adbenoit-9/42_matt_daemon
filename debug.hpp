/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 18:07:10 by adbenoit          #+#    #+#             */
/*   Updated: 2023/01/28 14:58:59 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP

# ifdef DEBUG
#  define PRINT_DEBUG(str, file, line)	do { printf("\033[31;1m%s:l.%d:\033[0m %s\n",\
											file, line, str); } while(0);
# else
#  define PRINT_DEBUG(str, file, line)
# endif

#endif
