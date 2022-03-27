#include "Pybind/Helpers/operatorNames.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <string>

namespace Pybind::Helpers {

std::optional<std::string> getOperatorName(IR::Operator op) {
	using IR::Operator;
	switch (op) {
		case Operator::Addition:
			// +
			return "__add__";
		case Operator::Subtraction:
			// -
			return "__sub__";
		case Operator::Multiplication:
			// *
			return "__mul__";
		case Operator::Division:
			// /
			return "__truediv__";
		case Operator::Modulus:
			// %
			return "__mod__";
		case Operator::Assignment:
			// =
			// No translation
			return std::nullopt;
		case Operator::Equal:
			// ==
			return "__eq__";
		case Operator::NotEqual:
			// !=
			return "__ne__";
		case Operator::GreaterThan:
			// >
			return "__gt__";
		case Operator::GreaterThanOrEqualTo:
			// >=
			return "__ge__";
		case Operator::LessThan:
			// <
			return "__lt__";
		case Operator::LessThanOrEqualTo:
			// <=
			return "__le__";
		case Operator::Subscript:
			// []
			return "__getitem__";
		case Operator::Call:
			// ()
			return "__call__";
	}
	return std::nullopt;
}

}    // namespace Pybind::Helpers
