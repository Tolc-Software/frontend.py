#pragma once

#include <optional>
#include <string>
#include <vector>

namespace PybindProxy {
class Function {
public:
	Function(std::string const& name, std::string const& fullyQualifiedName);

	/**
	* Creates a string corresponding to the pybind11 conversion of this function.
	* E.g.
	*   def("myFunction", &SomeNamespace::myFunction)
	*/
	std::string getPybind() const;

	/**
	* Adds an argument name.
	* E.g.
	*   f(int i) would require addArgument("i")
	*/
	void addArgument(std::string const& argument);

	void addInclude(std::string const& i);

	std::vector<std::string> const& getIncludes() const;

	// These are all 1-to-1 with pybind11
	enum class return_value_policy {
		take_ownership,
		copy,
		move,
		reference,
		reference_internal,
		automatic,
		automatic_reference
	};

	/**
	* Sets the return value policy for the function.
	* E.g.
	*   An input of "return_value_policy::take_ownership"
	*   would result in python calling delete on the object
	*   when it's garbage collected
	* NOTE: If not put in, the return_value_policy::automatic is used
	*/
	void setReturnValuePolicy(return_value_policy policy);

private:
	// User defined name of the function
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::optional<return_value_policy> m_returnValuePolicy;
	std::vector<std::string> m_arguments;
	std::vector<std::string> m_includes;
};
}
