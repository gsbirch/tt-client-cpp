#pragma once

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

#ifndef EXTERNALAPI_H
#define EXTERNALAPI_H

namespace tt
{
    /**
     * Represents the external API used for text embedding and completion.
     */
    class ExternalAPI {
        public:
            /** Used for checking whether a key has been set */
            const std::string INVALID_KEY = "API-INVALID-KEY";

            /** The URL to which large language model text embedding requests will be sent.*/
            const std::string EMBED_URL = "https://openrouter.ai/api/v1/embeddings";
            /** The model to be used for large language model text embedding requests. */
            const std::string EMBED_MODEL = "qwen/qwen3-embedding-8b";
            /** The maximum number of input tokens that can use for text embedding requests.*/
            const int EMBED_TOKENS_LIMIT = 1000000;

            /** The URL to which large language model text completion requests will be sent. */
            const std::string COMPLETE_URL = "https://openrouter.ai/api/v1/chat/completions";
            /** The large language model to be used for text completion requests. */
            const std::string COMPLETE_MODEL = "deepseek/deepseek-v4-flash-0731";
            /** The maximum number of input tokens that can use for text completion requests. */
            const int COMPLETE_IN_TOKENS_LIMIT = 1000000;
            /** The maximum number of output tokens that can use for text completion requests.*/
            const int COMPLETE_OUT_TOKENS_LIMIT = 1000000;
            /** The seed used for large language model text completion requests. */
            const int COMPLETE_SEED = 0;

            ExternalAPI(const std::string& key);

            ExternalAPI();

            /**
             * Queries the external text embedding API and returns its response as a
             * vector of floats. Each time this method is called, the number of tokens
             * used it added to `embed_tokens`. If the number of tokens used exceeds
             * `EMBED_TOKENS_LIMIT`, this method will raise an exception.
             * 
             * @param text The text which will be embedded as an array of floats.
             * @param dim The number of dimensions in the returned array.
             * @returns the vector embedding of the text as a vector of floats
             */
            std::vector<float> embed(std::string text, int dim = 1028);

            /**
             * Queries the external text completion API and returns its response as a
             * string. 
             * 
             * Each time this method is called, the number of tokens used
             * in the prompt is added to `complete_in_tokens` and the number of
             * tokens used in reasoning and completion is added to
             * `complete_out_tokens`. If the number of tokens used exceeds
             * `COMPLETE_IN_TOKENS_LIMIT` or `COMPLETE_OUT_TOKENS_LIMIT`
             * respectively, this method will raise an exception.
             * 
             * @param messages An array of strings representing a conversation.
             * The first string will be used as the system prompt. Every odd
             * numbered message will be treated as a message from the user. 
             * Every even numbered message (after the first) will be treated as 
             * a response from the assistant.
             * @returns The response for the text completion API as a string
             * @throws std::logic_error if the client does not have an API key
             */
            std::string complete(const std::vector<std::string>& messages);

            /**
             * Queries the external text completion API and returns its response as a
             * string. With a single string as input, the model will use its default 
             * system prompt and treat the string as a message from the user. 
             * 
             * Each time this method is called, the number of tokens used
             * in the prompt is added to `complete_in_tokens` and the number of
             * tokens used in reasoning and completion is added to
             * `complete_out_tokens`. If the number of tokens used exceeds
             * `COMPLETE_IN_TOKENS_LIMIT` or `COMPLETE_OUT_TOKENS_LIMIT`
             * respectively, this method will raise an exception.
             * 
             * @param messages the message from the user.
             * @returns The response for the text completion API as a string
             * @throws std::logic_error if the client does not have an API key
             */
            std::string complete(const std::string& message);

            /**
             * Sets the reasoning level used by the large language model during text completion.
             * Value includes `max`, `high`, `medium`, `low`, and `none`
             * 
             * To use the default value, you can pass `std::nullopt` as a parameter,
             * or pass no parameter at all.
             * @param value the reasoning level to use
             */
            void setReasoning(const std::optional<std::string>& value = std::nullopt);

            /**
             * Sets the maximum number of tokens the model may use to generate its response.
             * 
             * To use the default value, you can pass `std::nullopt` as a parameter,
             * or pass no parameter at all.
             * @param value the maxt to use
             */
            void setMaxT(const std::optional<int>& value = std::nullopt);

            /**
             * Sets the temperature, which influence the randomness of the response.
             * A value of 0.0 will give deterministic responses. The max value is 2.0.
             * 
             * To use the default value, you can pass `std::nullopt` as a parameter,
             * or pass no parameter at all.
             * @param value the temperature to use
             */
            void setTemp(const std::optional<float>& value = std::nullopt);

            /**
             * When choosing a next token to add to the output, 
             * the model may only choose from the best k tokens.
             * 
             * To use the default value, you can pass `std::nullopt` as a parameter,
             * or pass no parameter at all.
             * @param value the topk to use
             */
            void setTopK(const std::optional<int>& value = std::nullopt);

            /**
             * When choosing a next token to add to the output, the
             * model may only choose from tokens whose probability add to p.
             * 
             * To use the default value, you can pass `std::nullopt` as a parameter,
             * or pass no parameter at all.
             * @param value the topp to use
             */
            void setTopP(const std::optional<float>& value = std::nullopt);

            /**
             * When choosing a next token to add to the output, the
             * model may only choose from tokens whose probability is p times
             * the probability of the most likely token.
             * 
             * To use the default value, you can pass `std::nullopt` as a parameter,
             * or pass no parameter at all.
             * @param value the minp to use
             */
            void setMinP(const std::optional<float>& value = std::nullopt);

            /** Sets the API key.
             * 
             * @param k the key
             */
            void setKey(const std::string& k);

            /**
             * Gives the current reasoning value.
             * 
             * @return the current reasoning value
             */
            const std::optional<std::string>& getReasoning() const;

            /**
             * Gives the current maxt value.
             * 
             * @return the current maxt value
             */
            const std::optional<int>& getMaxT() const;

            /**
             * Gives the current temperature value.
             * 
             * @return the current temperature value
             */
            const std::optional<float>& getTemp() const;

            /**
             * Gives the current topk value.
             * 
             * @return the current topk value
             */
            const std::optional<int>& setTopK() const;

            /**
             * Gives the current topp value.
             * 
             * @return the current topp value
             */
            const std::optional<float>& getTopP() const;

            /**
             * Gives the current minp value.
             * 
             * @return the current minp value
             */
            const std::optional<float>& getMinP() const;

            /** Gives the API key.
             * 
             * @return the key
             */
            const std::string& getKey() const;

        private:
            /**
             * Raises an exception if the API key is not set.
             */
            void checkKey() const;

            /**
             * Raise an exception if the number of input tokens used exceeds the limit.
             * 
             * @param used the value to check
             * @param limit the limit to check
             */
            void checkInLimit(int used, int limit) const;

            /**
             * Raise an exception if the number of output tokens used exceeds the limit.
             * 
             * @param used the value to check
             * @param limit the limit to check
             */
            void checkOutLimit(int used, int limit) const;

            /**
             * Raise an exception if the value given is not a string from a pre-defined list.
             * 
             * @param name the name of the value being checked
             * @param value the value to check
             * @param values the `value` must be within this list of values
             */
            void checkNominal(const std::string& name, const std::string& value, const std::vector<std::string>& values) const;

            /**
             * Raise an exception if the given value is not an integer in the given range.
             * 
             * @param name the name of the value being checked
             * @param value the value to check
             * @param min the lower limit (inclusive)
             * @param max the upper limit (inclusive)
             */
            void checkInt(const std::string& name, int value, int min, int max);

            /**
             * Raise an exception if the given value is not a float in the given range.
             * 
             * @param name the name of the value being checked
             * @param value the value to check
             * @param min the lower limit (inclusive)
             * @param max the upper limit (inclusive)
             */
            void checkFloat(const std::string& name, float value, float min, float max);

            /** The client's API key */
            std::string key;

            /** The number of input tokens used so far by the text embedding model. */
            int embedTokens = 0;
            /** The number of input tokens used so far by the text completion model. */
            int completeInTokens = 0;
            /** The number of output toeksn used so far by the text completion model. */
            int completeOutTokens = 0;

            /** The level of reasoning used by the large language
                model during text completion. Value includes `max`, `high`,
                `medium`, `low`, and `none`. */
            std::optional<std::string> reasoning = std::nullopt;
            /** The maximum number of tokens the model may use to generate its response. */
            std::optional<int> maxt = std::nullopt;
            /** Influences the randomness of the response. */
            std::optional<float> temp = std::nullopt;
            /** When choosing a next token to add to the output, the model may only choose from the best k tokens. */
            std::optional<int> topk = std::nullopt;
            /** When choosing a next token to add to the output, the
                model may only choose from tokens whose probability add to p. */
            std::optional<float> topp = std::nullopt;
            /** When choosing a next token to add to the output, the
                model may only choose from tokens whose probability is p times
                the probability of the most likely token. */
            std::optional<float> minp = std::nullopt;
    };
} 


#endif