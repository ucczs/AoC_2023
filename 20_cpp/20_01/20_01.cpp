#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

auto split_string(std::string full_string, char seperator) -> std::vector<std::string> {
    std::stringstream full_string_ss(full_string);
    std::vector<std::string> splitted_string;
    std::string split;

    while (std::getline(full_string_ss, split, seperator)) {
        if (split[0] == ' ') {
            split = split.substr(1, split.length() - 1);
        }
        splitted_string.push_back(split);
    }

    return splitted_string;
}

enum class module_type { FLIP_FLOP, CONJUNCTION, BROADCAST, BUTTON, INVALID };
enum class pulse_t { HIGH, LOW, INVALID };

class Module {
   private:
    std::string name{""};
    module_type type{module_type::INVALID};
    std::vector<std::string> receiversStr;

   protected:
    pulse_t output_pulse{pulse_t::INVALID};
    std::vector<Module*> receivers{};
    int high_pulse_count{0};
    int low_pulse_count{0};
    std::map<std::string, pulse_t> senders;

   public:
    Module(std::string name, pulse_t initial_state) : name(name), output_pulse(initial_state) {};
    void addType(module_type type) { this->type = type; };
    void addReceiver(Module* receiver) { this->receivers.push_back(receiver); };
    void addReceiversStr(std::vector<std::string> receiversStr) { this->receiversStr = receiversStr; };

    std::string getName() const { return name; };
    pulse_t getOutput() const { return output_pulse; }
    const std::vector<std::string>& getReceiversStr() const { return receiversStr; };
    int getLowPulseCount() const { return low_pulse_count; };
    int getHighPulseCount() const { return high_pulse_count; };

    virtual void trigger(std::vector<Module&>& process_after_next) { std::runtime_error("Not implemented"); };
    void updateInput(const std::string& sender_name, const pulse_t pulse_type) { senders[sender_name] = pulse_type; };
};

class FlipFlop : public Module {
   public:
    FlipFlop(std::string name) : Module(name, pulse_t::LOW) {};

    virtual void trigger(std::vector<Module&>& process_after_next) override {
        pulse_t output_before = output_pulse;
        for (const auto& sender : senders) {
            if (sender.second == pulse_t::LOW) {
                output_pulse = (output_pulse == pulse_t::HIGH) ? pulse_t::LOW : pulse_t::HIGH;
            }
            break;
        }
        if (output_pulse == pulse_t::HIGH) {
            high_pulse_count++;
        } else {
            low_pulse_count++;
        }
        if (output_pulse != output_before) {
            for (auto receiver : receivers) {
                receiver->updateInput(this->getName(), output_pulse);
                process_after_next.push_back(*receiver);
                if (output_pulse == pulse_t::LOW) {
                    std::cout << this->getName() << " -low->" << receiver->getName() << std::endl;
                } else {
                    std::cout << this->getName() << " -high->" << receiver->getName() << std::endl;
                }
            }
        }
    }
};

class Conjunction : public Module {
   public:
    Conjunction(std::string name) : Module(name, pulse_t::HIGH) {};
    virtual void trigger(std::vector<Module&>& process_after_next) override {
        pulse_t output_before = output_pulse;

        bool allInputsHigh = true;
        for (auto iter = senders.begin(); iter != senders.end(); ++iter) {
            if (iter->second == pulse_t::LOW) {
                allInputsHigh = false;
            }
        }
        if (allInputsHigh) {
            output_pulse = pulse_t::LOW;
        } else {
            output_pulse = pulse_t::HIGH;
        }

        if (output_pulse == pulse_t::HIGH) {
            high_pulse_count++;
        } else {
            low_pulse_count++;
        }
        if (output_pulse != output_before) {
            for (auto& receiver : receivers) {
                receiver->updateInput(this->getName(), output_pulse);
                process_after_next.push_back(*receiver);
                if (output_pulse == pulse_t::LOW) {
                    std::cout << this->getName() << " -low->" << receiver->getName() << std::endl;
                } else {
                    std::cout << this->getName() << " -high->" << receiver->getName() << std::endl;
                }
            }
        }
    };
};

class Broadcaster : public Module {
   public:
    Broadcaster(std::string name) : Module(name, pulse_t::LOW) {};
    virtual void trigger(std::vector<Module&>& process_after_next) override {
        low_pulse_count++;
        std::cout << "button -low-> broadcaster" << std::endl;

        output_pulse = pulse_t::LOW;
        for (auto& receiver : receivers) {
            std::cout << this->getName() << " -low->" << receiver->getName() << std::endl;
            receiver->updateInput(this->getName(), output_pulse);
            process_after_next.push_back(*receiver);
        }
    }
};

class UntypedModule : public Module {
   public:
    UntypedModule(std::string name) : Module(name, pulse_t::LOW) {};
    virtual void trigger(std::vector<Module&>&) override {};
};

class CommuncationSetup {
   private:
    std::vector<std::unique_ptr<Module>> modules{};
    std::vector<Module&> process_next{};
    std::vector<Module&> process_after_next{};

   public:
    CommuncationSetup() {};
    void addModule(std::unique_ptr<Module> mod) {
        for (const auto& module : modules) {
            if (module->getName() == mod->getName()) {
                throw std::runtime_error("ERROR: Module already exists!");
            }
        }
        modules.push_back(std::move(mod));
    };

    void pushButton() {
        Module& broadcaster = getModuleByName("broadcaster");
        process_next.push_back(broadcaster);

        while (!process_next.empty()) {
            for (auto& process_module : process_next) {
                process_module.trigger(process_after_next);
            }
            process_next = process_after_next;
        }
    };

    Module& getModuleByName(std::string name) {
        for (const auto& module : modules) {
            if (module->getName() == name) {
                return *module;
            }
        }

        // element doesn't exists yet -> create an untyped module
        std::unique_ptr<UntypedModule> untyped_ptr = std::make_unique<UntypedModule>(name);
        this->addModule(std::move(untyped_ptr));
        for (const auto& module : modules) {
            if (module->getName() == name) {
                return *module;
            }
        }

        throw std::runtime_error("ERROR: Module cannot be found by name!");
    };

    void linkReceivers() {
        for (auto& module : modules) {
            const std::vector<std::string>& receiversStr = module->getReceiversStr();
            for (auto const& receiverStr : receiversStr) {
                Module& receiverMod = this->getModuleByName(receiverStr);
                module->addReceiver(&receiverMod);
                receiverMod.updateInput(module->getName(), module->getOutput());
            }
        }
    };

    int getResult() {
        int lowPulseCount = 0;
        int highPulseCount = 0;
        for (auto& module : modules) {
            lowPulseCount += module->getLowPulseCount();
            highPulseCount += module->getHighPulseCount();
        }

        std::cout << "Low pulses:\t" << lowPulseCount << std::endl;
        std::cout << "High pulses:\t" << highPulseCount << std::endl;

        return lowPulseCount * highPulseCount;
    };
};

int main() {
    // std::ifstream file("input.txt");
    std::ifstream file("test.txt");
    // std::ifstream file("test_02.txt");
    std::string line;

    CommuncationSetup setup{};

    while (std::getline(file, line)) {
        std::regex regex_pattern(R"((.*) -> (.*))");
        std::smatch matches;

        if (std::regex_search(line, matches, regex_pattern)) {
            module_type mod_type = module_type::INVALID;
            const std::string sender_str = matches[1].str();
            std::string sender_name = sender_str;
            if (sender_str.front() == '%') {
                mod_type = module_type::FLIP_FLOP;
                sender_name.erase(0, 1);
                std::unique_ptr<FlipFlop> flipFlop_ptr = std::make_unique<FlipFlop>(sender_name);
                setup.addModule(std::move(flipFlop_ptr));
            } else if (sender_str.front() == '&') {
                mod_type = module_type::CONJUNCTION;
                sender_name.erase(0, 1);
                std::unique_ptr<Conjunction> conjunction_ptr = std::make_unique<Conjunction>(sender_name);
                setup.addModule(std::move(conjunction_ptr));
            } else if (sender_str == "broadcaster") {
                mod_type = module_type::BROADCAST;
                std::unique_ptr<Broadcaster> broadcaster_ptr = std::make_unique<Broadcaster>(sender_name);
                setup.addModule(std::move(broadcaster_ptr));
            } else {
                throw std::runtime_error("ERROR: Type of module not defined!");
            }
            Module& mod_ref = setup.getModuleByName(sender_name);
            mod_ref.addType(mod_type);

            std::string receiver = matches[2].str();
            std::vector<std::string> receivers = split_string(receiver, ',');
            mod_ref.addReceiversStr(receivers);

        } else {
            std::runtime_error("ERROR: Read in regex failed!");
        }
    }
    setup.linkReceivers();

    constexpr int push_count = 1;
    for (size_t i = 0; i < push_count; i++) {
        setup.pushButton();
        std::cout << "-------------------" << std::endl;
    }

    int result = setup.getResult();

    std::cout << result << std::endl;

    file.close();
    return 0;
}