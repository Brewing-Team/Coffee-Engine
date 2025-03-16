.. _Styling:

Styling
=======

This style guide lists the conventions used in the Coffee-Engine project.
The goal is to make the codebase more readable and maintainable.
The project uses an auto-formating tool called clang-format to enforce a consistent coding style.

Hopefully, this guide will help you to understand the codebase and contribute to the project.

.. admonition:: Note
   :class: note

   Don't hesitate to look at the project's source code to see examples of how these conventions are applied.


General Rules
-------------

- Use English for all code, comments, and documentation.
- Use 4 spaces for indentation.
- Header files should have the .h extension, and source files should have the .cpp extension.
- Include guards should be used in header files.

.. code-block:: cpp
   :linenos:

    #pragma once


Includes
________

- The includes should be ordered: internal headers, external headers, standard library headers.
- The includes should be grouped by type and separated by a blank line.
- The includes should be at the top of the file.
- The includes can be at the .h or .cpp file, but it is recommended to put them in the .cpp file.

.. code-block:: cpp
    :linenos:
    
     // Includes
     #include "Coffee/Core/Log.h"
     #include "Coffee/Core/Application.h"

     #include <SDL3/SDL.h>
     #include <GLM/glm.hpp>
     #include <GLM/gtc/matrix_transform.hpp>

     #include <iostream>
     #include <vector>
     #include <string>
     #include <unordered_map>


Constants
_________

- Use constexpr instead of #define for constants.
- Use const for variables that are not meant to be modified.

.. code-block:: cpp
   :linenos:

    // Constexpr over Defines
    constexpr int MAX_SPEED = 5 /*over #define MAX_SPEED = 5*/

    // Constants
    const int SOME_NUMBER = 42;
    const std::string GREETING = "Hello, World!";


Enums
_____

.. admonition:: Note
   :class: danger

   This needs to be revised.

- Use enum class instead of enum.
- Use PascalCase for enum names.
- Use PascalCase for named enum values.
- Use UPPER_SNAKE_CASE for types enum values.

.. code-block:: cpp
   :linenos:

    enum class Named
    {
        Thing1,
        Thing2,
        AnotherThing = -1
    };

    enum class Types
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };


Classes
_______

- Use PascalCase for class names.
- Use camelCase with an initial lowercase "m" for member variables.
- Use PascalCase for member functions.
- Place public members first, followed by public protected and private functions, and finally protected and private members.

.. code-block:: cpp
    :linenos:
    
     class MyClass
     {
        public:
            //Public variables
            int PublicNumber = 5;

        public:
            // Constructor
            MyClass() : m_IntNumber(5), m_Name("Luigi Mangione 🫶"),
                        m_Numbers({1, 2, 3}), m_UnorderedMap({ {"key", 1} };)
            {
                /*  
                    Note that there is COFFEE_INFO and COFFEE_CORE_INFO.
                    COFFEE_INFO is used for the application and COFFEE_CORE_INFO for the engine.
                */

                COFFEE_CORE_INFO("Constructed!");
                COFFEE_INFO("My name is: {0}", m_Name);
            }

            // Example public function

            const std::vector<int>& SomeFunction(int first, int last)
            {
                std::vector<int> v;

                for (int i = first; i < last; ++i)
                {
                    v.push_back(i);
                }

                return v;
            }

            const void SetName(const std::string& name) { m_Name = name; }
            const std::string& GetName() const { return m_Name; }
        
        protected:
            void ProtectedFunction() { /* Do something */ }

        private:
            void PrivateFunction() { /* Do something */ }
        
        protected:
            bool m_ProctectedBool = false;
        
        private:
            int m_IntNumber;
            std::string m_Name;
            std::vector<int> m_Numbers;
            std::unordered_map<std::string, int> m_UnorderedMap;
     };
            

Conditionals
____________

.. code-block:: cpp
    :linenos:
    
     const int localConst = 5;
    
     if (param1 < localConst)
     {
          COFFEE_INFO("param1: {0}", param1);
     }
     else if (param2 > 5)
     {
          COFFEE_INFO("param2: {0}", param2);
     }
     else
     {
          COFFEE_ERROR("Fail!");
     }


Loops
_____

.. code-block:: cpp
    :linenos:
    
    // For loop
     for (int i = 0; i < 20; ++i)
     {
          COFFEE_INFO("Loop index: {0}", i);
     }

    // Range-based for loop
     vector<int> v = {1, 2, 3, 4, 5};
     for (int i : arr)
     {
          COFFEE_INFO("{0} ", i);
     }
    
    // While loop
     while (param2 != 0)
     {
          --param2;
     }

    // Iterators
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        COFFEE_INFO("{0} ", *it);
    }


Switch
______

- Use the `using enum` directive to avoid repeating the enum name in each case.
- Use braces for each case and default.
- Use a new line for each case and default.

.. code-block:: cpp
    :linenos:
    
    Types type = Types::PERSPECTIVE

    switch (type)
    {
        using enum Types;
        case PERSPECTIVE:
        {
            COFFEE_INFO("type is PERSPECTIVE!");
            break;
        }
        case ORTHOGRAPHIC:
        {
            COFFEE_INFO("type is ORTHOGRAPHIC!");
        }
        default:
        {
            COFFEE_INFO("type is not a type of the enum!");
        }
    }

.. admonition:: Note
   :class: note

   If you have any questions or suggestions, don't hesitate to tell us in the #coding channel on Discord.