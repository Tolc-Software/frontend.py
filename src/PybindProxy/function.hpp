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
	*   f(int i) would require addArgument("int", "i")
	*/
	void addArgument(std::string const& typeName, std::string const& name = "");

	void addInclude(std::string const& i);

	void setAsConstructor();

	// Is there another function with the same name but with different arguments?
	void setAsOverloaded();

	// E.g. std::string f();
	// should call setReturnType("std::string")
	void setReturnType(std::string const& typeName);

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
	std::string getSignature() const;
	std::string getArgumentTypes() const;

	struct Argument {
		// E.g.
		//   f(int i)
		// Would result in
		//   typeName = "int"
		//   name = "i"
		std::string typeName;
		std::string name;
	};

	// User defined name of the function
	std::string m_name;
	std::string m_fullyQualifiedName;
	// Defaults to void
	std::string m_returnType;
	std::optional<return_value_policy> m_returnValuePolicy;
	std::vector<Argument> m_arguments;
	std::vector<std::string> m_includes;
	bool m_isConstructor;
	bool m_isOverloaded;
};
}
