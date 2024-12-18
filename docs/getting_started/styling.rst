.. _Styling:

Styling
=======

The project uses clang-format to enforce a consistent coding style. There is also this code block in this website as an example of the style used in the project.

.. code-block:: cpp
   :linenos:

    // Includes
    #include <iostream>
    #include <vector>
    #include <string>
    #include <unordered_map>

    // Constexpr over Defines
    constexpr int MAX_SPEED = 5 /*over #define MAX_SPEED = 5*/

    // Constants
    const int SOME_NUMBER = 42;
    const std::string GREETING = "Hello, World!";

    // Enums
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

    // Example Class
    class MyClass
    {
        public:
            //Public variables
            int PublicNumber = 5;

        public:
            // Constructor
            MyClass() : m_IntNumber(5), m_Name("Luigi Mangione 🫶"),
                        m_Numbers({1, 2, 3}), m_Dictionary({ {"key", 1} };)
            {
                // Note that there is COFFEE_INFO and COFFEE_CORE_INFO
                // depending on the case you should use the proper one.
                COFFEE_CORE_INFO("Constructed!");
                COFFEE_INFO("{0}, me caso", m_Name);
            }

            // Example function
            int SomeFunction(int param1, int param2, int param3)
            {
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

                for (int i = 0; i < 20; ++i)
                {
                    COFFEEINFO("Loop index: {0}", i);
                }

                vector<int> v = {1, 2, 3, 4, 5};
                for (int i : arr)
                {
                    COFFEE_INFO("{0} ", i);
                }

                while (param2 != 0)
                {
                    --param2;
                }

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

                return param1 + 3;
            }

            // Nested class
            class NestedClass
            {
                public:
                int nestedValue = 10;
            };

            // Function overriding
            virtual void Something(int p1, int p2) override;
        private:
            // Member variables
            int m_IntNumber;
            std::string m_Name;
            std::vector<int> m_Numbers;
            std::unordered_map<std::string, int> m_Dictionary;
    };