/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_decorator_factory.h"

#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

ReplyDecoratorFactory::ReplyDecoratorFactory(QObject* parent)
    : QObject(parent) {}

ReplyDecoratorFactory::ReplyDecoratorFactory(QObjectPrivate& impl,
                                             QObject* parent)
    : QObject(impl, parent) {}

ReplyDecoratorFactory::~ReplyDecoratorFactory() = default;

}  // namespace egnite::rest
