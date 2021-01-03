#include "Trit.h"

trit operator!(trit x)
{
	switch(x)
	{
		case trit::False:
			return trit::True;
		case trit::Unknown:
			return trit::Unknown;
		case trit::True:
			return trit::False;
	}
	return trit::Unknown;
}

trit operator|(trit x, trit y)
{
	switch (x)
	{
		case trit::False:
			switch (y) {
				case trit::False:
					return trit::False;
				case trit::Unknown:
					return trit::Unknown;
				case trit::True:
					return trit::True;
			}
		case trit::Unknown:
			switch (y) {
				case trit::False:
					return trit::Unknown;
				case trit::Unknown:
					return trit::Unknown;
				case trit::True:
					return trit::True;
			}
		case trit::True:
			return trit::True;
	}
	return trit::Unknown;
}

trit operator&(trit x, trit y)
{
	switch (x)
	{
		case trit::False:
			return trit::False;
		case trit::Unknown:
			switch (y)
			{
				case trit::False:
					return trit::False;
				case trit::Unknown:
					return trit::Unknown;
				case trit::True:
					return trit::Unknown;
			}
		case trit::True:
			switch (y)
			{
				case trit::False:
					return trit::False;
				case trit::Unknown:
					return trit::Unknown;
				case trit::True:
					return trit::True;
			}
	}
	return trit::Unknown;
}

std::ostream& operator<<(std::ostream& os, trit x)
{
	switch (x)
	{
	case trit::False:
		return os << "False";
	case trit::Unknown:
		return os << "Unknown";
	case trit::True:
		return os << "True";
	}
	return os << "Unknown";
}
