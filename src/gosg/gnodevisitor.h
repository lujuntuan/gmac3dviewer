/*********************************************************************************
 *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
 *Author:  Juntuan.Lu
 *Version: 1.0
 *Date:  2021/10/23
 *Email: 931852884@qq.com
 *Description:
 *Others:
 *Function List:
 *History:
 **********************************************************************************/

#ifndef GNODEVISITOR_H
#define GNODEVISITOR_H

#include <iostream>
#include <osg/NodeVisitor>

class GNameNodeVisitor : public osg::NodeVisitor {
public:
    explicit GNameNodeVisitor()
        : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
    {
    }
    osg::ref_ptr<osg::Node> getNode(const osg::ref_ptr<osg::Node>& node, const std::string& name)
    {
        m_name = name;
        this->apply(*node);
        return m_node;
    }

protected:
    virtual void apply(osg::Node& node) override
    {
        if (node.getName() == m_name) {
            m_node = &node;
            return;
        }
        traverse(node);
    }

private:
    std::string m_name;
    osg::ref_ptr<osg::Node> m_node = nullptr;
};

template <typename TARGET, typename BASE>
class GAnimationNodeVisitor : public osg::NodeVisitor {
public:
    explicit GAnimationNodeVisitor()
        : osg::NodeVisitor(TRAVERSE_NONE)
    {
    }
    osg::ref_ptr<TARGET> getNode(const osg::ref_ptr<osg::Node>& node)
    {
        osg::ref_ptr<TARGET> manager;
        this->apply(*node);
        if (m_node.valid()) {
            manager = new TARGET(*m_node);
            node->setUpdateCallback(manager);
        }
        return manager;
    }

protected:
    virtual void apply(osg::Node& node) override
    {
        if (node.getUpdateCallback()) {
            m_node = dynamic_cast<BASE*>(node.getUpdateCallback());
            return;
        }
        traverse(node);
    }

private:
    osg::ref_ptr<BASE> m_node = nullptr;
};

#endif // GNODEVISITOR_H
