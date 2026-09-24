#include <tt/io/MessageFactory.h>
#include <mutex>
#include <tt/io/Connect.h>
#include <tt/io/Join.h>
#include <tt/io/Start.h>
#include <tt/io/Update.h>
#include <tt/io/Choice.h>
#include <tt/io/Report.h>
#include <tt/io/Stop.h>
#include <tt/io/End.h>
#include <tt/io/Error.h>


void tt::registerMessageTypes()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        MessageFactory::instance().registerType("Connect", &createFromJson<Connect>);
        MessageFactory::instance().registerType("Join", &createFromJson<Join>);
        MessageFactory::instance().registerType("Start", &createFromJson<Start>);
        MessageFactory::instance().registerType("Update", &createFromJson<Update>);
        MessageFactory::instance().registerType("Choice", &createFromJson<Choice>);
        MessageFactory::instance().registerType("Report", &createFromJson<Report>);
        MessageFactory::instance().registerType("Stop", &createFromJson<Stop>);
        MessageFactory::instance().registerType("End", &createFromJson<End>);
        MessageFactory::instance().registerType("Error", &createFromJson<Error>);
    });
}
