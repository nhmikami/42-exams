# include <stdarg.h>
# include <unistd.h>

int print_chr(char c)
{
	write(1, &c, 1);
	return (1);
}

int	print_str(char *str)
{
	int	count = 0;

	if (!str)
		return (print_str("(null)"));
	while (*str)
	{
		count += print_chr(*str);
		str++;
	}
	return (count);
}

int	print_nbr(int nbr)
{
	long	num = (long)nbr;
	int		count = 0;

	if (num < 0)
	{
		count += print_chr('-');
		num = -num;
	}
	if (num >= 10)
	{
		count += print_nbr(num / 10);
		count += print_nbr(num % 10);
	}
	else
		count += print_chr(num + '0');		
	return (count);
}

int	print_hex(unsigned int n, const char *base)
{
	int			count;

	count = 0;
	if (n >= 16)
	{
		count += print_hex(n / 16, base);
		count += print_hex(n % 16, base);
	}
	else
		count += print_chr(base[n]);
	return (count);
}

int print_arg(va_list args, char spec)
{
	int count = 0;

	if (spec == 's')
		count += print_str(va_arg(args, char *));
	else if (spec == 'd')
		count += print_nbr(va_arg(args, int));
	else if (spec == 'x')
		count += print_hex(va_arg(args, unsigned int), "0123456789abcdef");
	else if (spec == 'X')
		count += print_hex(va_arg(args, unsigned int), "0123456789ABCDEF");
	return (count);
}

int ft_printf(const char *format, ...)
{
	va_list args;
	int count = 0;

	if (!format)
		return (-1);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			count += print_arg(args, *format);
			format++;
		}
		else
		{
			count += print_chr(*format);
			format++;
		}
	}
	va_end(args);
	return (count);
}

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
int	main(void)
{
	ft_printf("%s\n", "toto");
	ft_printf("Magic %s is %d\n", "number", 42);
	ft_printf("Hexadecimal for %d is %x\n", 42, 42);

	printf("\n- string\n");
	char	*s = 0;
	printf(" count = %i\n", ft_printf("string: %s!", "oi"));
	printf(" count = %i\n", printf("string: %s!", "oi"));
	printf(" count = %i\n", ft_printf("string: %s!", s));
	printf(" count = %i\n", printf("string: %s!", s));

	printf("\n- integer / double\n");
	printf("i, 0\n");
	printf(" count = %i\n", ft_printf("int: %d!", 0));
	printf(" count = %i\n", printf("int: %d!", 0));
	printf("i, 42\n");
	printf(" count = %i\n", ft_printf("int: %d!", 42));
	printf(" count = %i\n", printf("int: %d!", 42));
	printf("i, -42\n");
	printf(" count = %i\n", ft_printf("int: %d!", -42));
	printf(" count = %i\n", printf("int: %d!", -42));
	printf("d, INT_MAX\n");
	printf(" count = %i\n", ft_printf("int: %d!", INT_MAX));
	printf(" count = %i\n", printf("int: %d!", INT_MAX));
	printf("d, INT_MIN\n");
	printf(" count = %i\n", ft_printf("int: %d!", INT_MIN));
	printf(" count = %i\n", printf("int: %d!", INT_MIN));

	printf("\n- hexadecimal\n");
	printf("x, 0\n");
	printf(" count = %i\n", ft_printf("hexadecimal uppercase: %x!", 0));
	printf(" count = %i\n", printf("hexadecimal uppercase: %x!", 0));
	printf("x, 255\n");
	printf(" count = %i\n", ft_printf("hexadecimal lowercase: %x!", 255));
	printf(" count = %i\n", printf("hexadecimal lowercase: %x!", 255));
	printf("X, 255\n");
	printf(" count = %i\n", ft_printf("hexadecimal uppercase: %X!", 255));
	printf(" count = %i\n", printf("hexadecimal uppercase: %X!", 255));
	printf("X, -255\n");
	printf(" count = %i\n", ft_printf("hexadecimal uppercase: %X!", -255));
	printf(" count = %i\n", printf("hexadecimal uppercase: %X!", -255));

	return (0);
}
