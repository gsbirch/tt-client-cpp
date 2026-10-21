#include <tt/ExternalAPI.h>
#include <stdexcept>
#include <cpr/cpr.h>
#include <iostream>
#include <tt/util/Util.h>
#include <algorithm>

using namespace tt;

tt::ExternalAPI::ExternalAPI(const std::string &key):
key(key), embedTokens(0), completeInTokens(0), completeOutTokens(0),
reasoning(std::nullopt), maxt(std::nullopt), temp(std::nullopt), topk(std::nullopt), topp(std::nullopt), minp(std::nullopt)
{
}

tt::ExternalAPI::ExternalAPI():
ExternalAPI(INVALID_KEY)
{
}

std::vector<float> tt::ExternalAPI::embed(std::string text, int dim)
{
    // Check if this request is possible.
    checkKey();
    checkInLimit(embedTokens, EMBED_TOKENS_LIMIT);
    // Construct the data object.
    nlohmann::json data = {
        {"model", EMBED_MODEL},
        {"input_type", "string"},
        {"input", text},
        {"encoding_format", "float"}
    };
    if (dim != -1) {
        requireNonNegative(dim, "dimension count");
        data["dimensions"] = dim;
    }
    // Send the request.
    cpr::Response r = cpr::Post(
        cpr::Url{EMBED_URL},
        cpr::Header{{"Content-Type", "application/json"},
                    {"Authorization", "Bearer " + key}},
        cpr::Body{data.dump()},
        cpr::Timeout{10000}
    );

    // Process the response.
    if (!cpr::status::is_success(r.status_code)) {
        std::cout << "HTTP " + std::to_string(r.status_code) + ": " + r.text << std::endl;
        throw std::runtime_error(
        "HTTP " + std::to_string(r.status_code) + ": " + r.text);
    }
    if (r.status_code == 200) {
        nlohmann::json responseData = nlohmann::json::parse(r.text);
        embedTokens += responseData["usage"]["total_tokens"].get<int>();
        return responseData["data"][0]["embedding"].get<std::vector<float>>();
    }
    return std::vector<float>();
}

std::string tt::ExternalAPI::complete(const std::vector<std::string> &messages)
{
    // Check if this request is possible.
    checkKey();
    checkInLimit(completeInTokens, COMPLETE_IN_TOKENS_LIMIT);
    checkInLimit(completeOutTokens, COMPLETE_OUT_TOKENS_LIMIT);

    std::vector<nlohmann::json> msg;
    // If messages is a single string, use no system prompt.
    if (messages.size() == 1) {
        msg.push_back({{"role", "user"}, {"content", messages[0]}});
    } 
    else {
        if (messages.size() % 2 == 1) {
            throw std::length_error("Conversation must have an even number of messages.");
        }
        msg.push_back({{"role", "system"}, {"content", messages[0]}});
        for (int i = 1; i < messages.size(); i++) {
            if (i % 2 == 0)
                msg.push_back({{"role", "user"}, {"content", messages[i]}});
            else
                msg.push_back({{"role", "assistant"}, {"content", messages[i]}});
        }
    }
    
    nlohmann::json data = {
        {"model", COMPLETE_MODEL},
        {"messages", msg},
        {"seed", COMPLETE_SEED},
    };
    if (reasoning != std::nullopt) {
        data["reasoning_effort"] = reasoning;
    }
    if (maxt != std::nullopt) {
        data["max_completion_tokens"] = maxt;
    }
    if (temp != std::nullopt) {
        data["temperature"] = temp;
    }
    if (topk != std::nullopt) {
        data["top_k"] = topk;
    }
    if (topp != std::nullopt) {
        data["top_p"] = topp;
    }
    if (minp != std::nullopt) {
        data["min_p"] = minp;
    }
    // Send the request.
    cpr::Response r = cpr::Post(
        cpr::Url{COMPLETE_URL},
        cpr::Header{{"Content-Type", "application/json"},
                    {"Authorization", "Bearer " + key}},
        cpr::Body{data.dump()},
        cpr::Timeout{10000}
    );
    // Process the response.
    if (!cpr::status::is_success(r.status_code)) {
        std::cout << "HTTP " + std::to_string(r.status_code) + ": " + r.text << std::endl;
        throw std::runtime_error(
        "HTTP " + std::to_string(r.status_code) + ": " + r.text);
    }
    if (r.status_code == 200) {
        nlohmann::json responseData = nlohmann::json::parse(r.text);
        completeInTokens += responseData["usage"]["prompt_tokens"].get<int>();
        completeOutTokens += responseData["usage"]["completion_tokens"].get<int>();
        return responseData["choices"][0]["message"]["content"].get<std::string>();
    }

    return "";
}

std::string tt::ExternalAPI::complete(const std::string &message)
{
    std::vector<std::string> msgs = {message};
    return complete(msgs);
}

void tt::ExternalAPI::setReasoning(const std::optional<std::string> &value)
{
    if (value)
        checkNominal("Reasoning level", *value, {"max", "high", "medium", "low", "none"});
    reasoning = value;
}

void tt::ExternalAPI::setMaxT(const std::optional<int> &value)
{
    maxt = value;
}

void tt::ExternalAPI::setTemp(const std::optional<float> &value)
{
    if (value)
        checkFloat("Temperature", *value, 0.0, 2.0);
    temp = value;
}

void tt::ExternalAPI::setTopK(const std::optional<int> &value)
{
    topk = value;
}

void tt::ExternalAPI::setTopP(const std::optional<float> &value)
{
    if (value)
        checkFloat("Top P", *value, 0.0, 1.0);
    topp = value;
}

void tt::ExternalAPI::setMinP(const std::optional<float> &value)
{
    if (value)
        checkFloat("Min P", *value, 0.0, 1.0);
    minp = value;
}

void tt::ExternalAPI::setKey(const std::string &k)
{
    key = k;
}

const std::optional<std::string> &tt::ExternalAPI::getReasoning() const
{
    return reasoning;
}

const std::optional<int> &tt::ExternalAPI::getMaxT() const
{
    return maxt;
}

const std::optional<float> &tt::ExternalAPI::getTemp() const
{
    return temp;
}

const std::optional<int> &tt::ExternalAPI::getTopK() const
{
    return topk;
}

const std::optional<float> &tt::ExternalAPI::getTopP() const
{
    return topp;
}

const std::optional<float> &tt::ExternalAPI::getMinP() const
{
    return minp;
}

const std::string &tt::ExternalAPI::getKey() const
{
    return key;
}

const int tt::ExternalAPI::getRemainingInTokens() const
{
    return std::max(0, COMPLETE_IN_TOKENS_LIMIT - completeInTokens);
}

const int tt::ExternalAPI::getRemainingOutTokens() const
{
    return std::max(0, COMPLETE_OUT_TOKENS_LIMIT - completeOutTokens);
}

const int tt::ExternalAPI::getRemainingEmbedTokens() const
{
    return std::max(0, EMBED_TOKENS_LIMIT - embedTokens);
}

const int tt::ExternalAPI::getInTokensUsed() const
{
    return completeInTokens;
}

const int tt::ExternalAPI::getOutTokensUsed() const
{
    return completeOutTokens;
}

const int tt::ExternalAPI::getEmbedTokensUsed() const
{
    return embedTokens;
}

void tt::ExternalAPI::checkKey() const
{
    if(key == INVALID_KEY)
        throw std::logic_error("The client does not have an API key, so it cannot use the external API.");
}

void tt::ExternalAPI::checkInLimit(int used, int limit) const
{
    if (used >= limit)
        throw std::length_error("The input token limit " + std::to_string(limit) + " has been exceeded.");
}

void tt::ExternalAPI::checkOutLimit(int used, int limit) const
{
    if (used >= limit)
        throw std::length_error("The output token limit " + std::to_string(limit) + " has been exceeded.");
}

void tt::ExternalAPI::checkNominal(const std::string &name, const std::string &value, const std::vector<std::string> &values) const
{
        std::string names = "[";
        for (int i = 0; i < values.size(); i++) { 
            names += values[i];
            if (i != values.size() - 1)
                names += ", ";
        }
        names += "]";
        throw std::logic_error(name + " must be one of " + names);

}

void tt::ExternalAPI::checkInt(const std::string &name, int value, int min, int max)
{
    if (value < min)
        throw std::logic_error(name + " must be >= " + std::to_string(min));
    if (value > max)
        throw std::logic_error(name + " must be <= " + std::to_string(max));
}

void tt::ExternalAPI::checkFloat(const std::string &name, float value, float min, float max)
{
    if (value < min)
        throw std::logic_error(name + " must be >= " + std::to_string(min));
    if (value > max)
        throw std::logic_error(name + " must be <= " + std::to_string(max));
}
