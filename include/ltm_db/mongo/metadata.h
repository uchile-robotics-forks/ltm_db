/*
 * Copyright (c) 2008, Willow Garage, Inc.
 * All rights reserved.
 */

/**
 * \file 
 * 
 * Define a couple of classes that wrap Mongo's BSON type
 *
 * \author Bhaskara Marthi
 */

#ifndef LTM_DB_MONGO_METADATA_H
#define LTM_DB_MONGO_METADATA_H

// We have to include this top-level include here because
// the mongo c++ library is not robust to reincludes
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include <ltm_db/mongo/config.h>
#include <ltm_db/interface/metadata.h>

namespace ltm_db_mongo
{

using mongo::BSONObj;
using mongo::BSONObjBuilder;

/// \brief Internal parent class
///
/// This allows the user to not have to deal with separate BSONObj and
/// BSONObj builder objects
class WrappedBSON : public BSONObj
{
public:
  WrappedBSON() :
    BSONObj(), builder_(new BSONObjBuilder())
  {}

  WrappedBSON(const WrappedBSON& other) :
    BSONObj(), builder_(other.builder_)
  {
    update();
  }

  WrappedBSON(const BSONObj& other) :
    BSONObj(), builder_(new BSONObjBuilder())
  {
    builder_->appendElements(other);
    update();
  }

  WrappedBSON(const std::string& json) :
    BSONObj(), builder_(new BSONObjBuilder())
  {
    builder_->appendElements(mongo::fromjson(json.c_str()));
    update();
  }

protected:
  boost::shared_ptr<BSONObjBuilder> builder_;

  void update()
  {
    BSONObj::operator=(builder_->asTempObj());
  }
};


/// \brief Represents a query to the db
///
/// Usage:
/// Query q("foo", 42);
/// Query q2("bar", LT, 24); // bar less than 24
/// Templated so you can have different types of values
///
/// Or:
/// q = Query().append("foo", 42).append("bar", LT, 24);
class MongoQuery : public WrappedBSON, public ltm_db::Query
{
public:
  MongoQuery() : WrappedBSON ()
  {}

  MongoQuery(const MongoQuery& other) :
    WrappedBSON(other)
  {}

  MongoQuery(const BSONObj& other) :
    WrappedBSON(other)
  {}

  void append(const std::string& name,
              const std::string& val)
  {
    ROS_DEBUG_STREAM("Append string: " << name << ", " << val);
    *builder_ << name << val;
    WrappedBSON::update();
  }

  void append(const std::string& name,
              const double val)
  {
    ROS_DEBUG_STREAM("Append double: " << name << ", " << val);
    *builder_ << name << val;
    WrappedBSON::update();
  }

  void append(const std::string& name,
              const int val)
  {
    ROS_DEBUG_STREAM("Append int: " << name << ", " << val);
    *builder_ << name << val;
    WrappedBSON::update();
  }

  void append(const std::string& name,
              const bool val)
  {
    ROS_DEBUG_STREAM("Append bool: " << name << ", " << val);
    *builder_ << name << val;
    WrappedBSON::update();
  }

  void appendLT(const std::string& name,
                const double val)
  {
    ROS_DEBUG_STREAM("Append LT double: " << name << ", " << val);
    *builder_ << name << mongo::LT << val;
    WrappedBSON::update();
  }

  void appendLT(const std::string& name,
                const int val)
  {
    ROS_DEBUG_STREAM("Append LT string: " << name << ", " << val);
    *builder_ << name << mongo::LT << val;
    WrappedBSON::update();
  }

  void appendLTE(const std::string& name,
                 const double val)
  {
    ROS_DEBUG_STREAM("Append LTE double: " << name << ", " << val);
    *builder_ << name << mongo::LTE << val;
    WrappedBSON::update();
  }

  void appendLTE(const std::string& name,
                 const int val)
  {
    ROS_DEBUG_STREAM("Append LTE int: " << name << ", " << val);
    *builder_ << name << mongo::LTE << val;
    WrappedBSON::update();
  }

  void appendGT(const std::string& name,
                const double val)
  {
    ROS_DEBUG_STREAM("Append GT double: " << name << ", " << val);
    *builder_ << name << mongo::GT << val;
    WrappedBSON::update();
  }

  void appendGT(const std::string& name,
                const int val)
  {
    ROS_DEBUG_STREAM("Append GT double: " << name << ", " << val);
    *builder_ << name << mongo::GT << val;
    WrappedBSON::update();
  }

  void appendGTE(const std::string& name,
                 const double val)
  {
    ROS_DEBUG_STREAM("Append GTE double: " << name << ", " << val);
    *builder_ << name << mongo::GTE << val;
    WrappedBSON::update();
  }

  void appendGTE(const std::string& name,
                 const int val)
  {
    ROS_DEBUG_STREAM("Append GTE int: " << name << ", " << val);
    *builder_ << name << mongo::GTE << val;
    WrappedBSON::update();
  }

  void appendRange(const std::string& name,
                   const double lower,
                   const double upper)
  {
    ROS_DEBUG_STREAM("Append range double: " << name << " from " << lower << " to " << upper);
    *builder_ << name << mongo::GT << lower << mongo::LT << upper;
    WrappedBSON::update();
  }

  void appendRange(const std::string& name,
                   const int lower,
                   const int upper)
  {
    ROS_DEBUG_STREAM("Append range int: " << name << " from " << lower << " to " << upper);
    *builder_ << name << mongo::GT << lower << mongo::LT << upper;
    WrappedBSON::update();
  }

  void appendRangeInclusive(const std::string& name,
                            const double lower,
                            const double upper)
  {
    ROS_DEBUG_STREAM("Append range double inclusive: " << name << " from " << lower << " to " << upper);
    *builder_ << name << mongo::GTE << lower << mongo::LTE << upper;
    WrappedBSON::update();
  }

  void appendRangeInclusive(const std::string& name,
                            const int lower,
                            const int upper)
  {
    ROS_DEBUG_STREAM("Append range int inclusive: " << name << " from " << lower << " to " << upper);
    *builder_ << name << mongo::GTE << lower << mongo::LTE << upper;
    WrappedBSON::update();
  }
};


/// \brief Represents metadata attached to a message.  Automatically
/// includes a unique id and creation time.
///
/// Usage:
///
/// Metadata m("x", 24, "y", 42);
/// (templated so you can use varying number of fields, numeric or string values)
/// 
/// Or:
/// m = Metadata().append("x", 24).append("name", "foo");
class MongoMetadata : public ltm_db::Metadata, public WrappedBSON
{
public:
  MongoMetadata() :
    WrappedBSON ()
  {
    initialize();
  }

  MongoMetadata(const std::string& json) :
    WrappedBSON (json)
  {}

  MongoMetadata(const MongoMetadata& other) :
    WrappedBSON(other)
  {}

  MongoMetadata(const BSONObj& other) :
    WrappedBSON(other)
  {}

  void append(const std::string& name,
              const std::string& val)
  {
    *builder_ << name << val;
    WrappedBSON::update();
  }

  void append(const std::string& name,
              const double val)
  {
    *builder_ << name << val;
    WrappedBSON::update();
  }

  void append(const std::string& name,
              const int val)
  {
    *builder_ << name << val;
    WrappedBSON::update();
  }

  void append(const std::string& name,
              const bool val)
  {
    *builder_ << name << val;
    WrappedBSON::update();
  }

  std::string lookupString(const std::string& name) const
  {
    return getStringField(name.c_str());
  }

  double lookupDouble(const std::string& name) const
  {
    double d;
    (*this)[name.c_str()].Val(d);
    return d;
  }

  int lookupInt(const std::string& name) const
  {
    return getIntField(name.c_str());
  }

  bool lookupBool(const std::string& name) const
  {
    return getBoolField(name.c_str());
  }

  bool lookupField(const std::string& name) const
  {
    return BSONObj::hasField(name.c_str());
  }

  std::set<std::string> lookupFieldNames() const
  {
    std::set<std::string> fields;
    BSONObj::getFieldNames(fields);
    return fields;
  }

private:
  void initialize()
  {
    builder_->genOID();
    WrappedBSON::update();
  }
};


} // namespace

#endif // include guard