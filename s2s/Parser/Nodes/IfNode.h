//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_IFNODE_H
#define S2S_IFNODE_H


class IfNode : public ParserNode {
public:
    ParserNode *condition;
    std::vector<ParserNode *> body;
    std::vector<IfNode *> elseIfs;
    std::vector<ParserNode *> elseBody;

    IfNode(ParserNode *condition, std::vector<ParserNode *> body) : ParserNode(NodeType::If), condition(condition),
                                                                    body(std::move(body)) {}

    IfNode(ParserNode *condition, std::vector<ParserNode *> body, std::vector<ParserNode *> elseBody) :
            ParserNode(NodeType::If), condition(condition), body(std::move(body)), elseBody(std::move(elseBody)) {}

    IfNode(ParserNode *condition, std::vector<ParserNode *> body, std::vector<IfNode *> elseIfs,
           std::vector<ParserNode *> elseBody) : ParserNode(NodeType::If), condition(condition), body(std::move(body)),
                                                 elseIfs(std::move(elseIfs)), elseBody(std::move(elseBody)) {}
};


#endif //S2S_IFNODE_H
