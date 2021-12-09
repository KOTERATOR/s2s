//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_FUNCTIONCALLNODE_H
#define S2S_FUNCTIONCALLNODE_H


class FunctionCallNode : public ParserNode {
public:
    std::string functionName;
    std::vector<ParserNode *> args;

    FunctionCallNode(std::string functionName, std::vector<ParserNode *> args) : ParserNode(NodeType::FunctionCall) {
        this->functionName = std::move(functionName);
        this->args = std::move(args);
    }

    std::string toString() override {
        std::string argsStr = "[";
        for (int i = 0; i < args.size(); i++) {
            argsStr += args[i]->toString();
            if (i < args.size() - 1)
                argsStr += ", ";
        }
        argsStr += "]";
        return std::string("FunctionCallNode: name=") + functionName + std::string(" args=") + argsStr;
    }
};

#endif //S2S_FUNCTIONCALLNODE_H
