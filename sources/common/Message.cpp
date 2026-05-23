#include "common/Message.hpp"

fus::common::ID &fus::common::Message::id()
{
    return _id;
}

const fus::common::ID & fus::common::Message::id() const
{
    return _id;
}

void fus::common::Message::setId(ID id)
{
    _id = id;
}

std::vector<fus::common::Byte> &fus::common::Message::body()
{
    return _body;
}

const std::vector<fus::common::Byte> &fus::common::Message::body() const
{
    return _body;
}

void fus::common::Message::setBody(const std::vector<Byte> &body)
{
    _body = body;
}