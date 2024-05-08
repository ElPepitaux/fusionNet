/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** Error
*/

#ifndef ERROR_HPP_
    #define ERROR_HPP_

    #include <iostream>
    #include <exception>

class Error : public std::exception {
    public:
        Error(const std::string &message) : m_message(message) {}
        virtual ~Error() = default;

        const char *what() const noexcept override { return m_message.c_str(); }

    protected:
        const std::string m_message;
    private:
};

#endif /* !ERROR_HPP_ */
