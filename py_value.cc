using namespace boost::python;
using namespace ledger;

long	 balance_len(balance_t& bal);
amount_t balance_getitem(balance_t& bal, int i);
long	 balance_pair_len(balance_pair_t& bal_pair);
amount_t balance_pair_getitem(balance_pair_t& bal_pair, int i);

long value_len(value_t& val)
{
  switch (val.type) {
  case value_t::BOOLEAN:
  case value_t::INTEGER:
  case value_t::DATETIME:
  case value_t::AMOUNT:
    return 1;

  case value_t::BALANCE:
    return balance_len(*((balance_t *) val.data));

  case value_t::BALANCE_PAIR:
    return balance_pair_len(*((balance_pair_t *) val.data));

  case value_t::STRING:
  case value_t::XML_NODE:
  case value_t::POINTER:
    return 1;

  case value_t::SEQUENCE:
    return (*(value_t::sequence_t **) val.data)->size();

  default:
    assert(0);
    break;
  }
  assert(0);
  return 0;
}

amount_t value_getitem(value_t& val, int i)
{
  std::size_t len = value_len(val);

  if (abs(i) >= len) {
    PyErr_SetString(PyExc_IndexError, "Index out of range");
    throw_error_already_set();
  }

  switch (val.type) {
  case value_t::BOOLEAN:
    throw_(value_exception, "Cannot cast a boolean to an amount");

  case value_t::INTEGER:
    return long(val);

  case value_t::DATETIME:
    throw_(value_exception, "Cannot cast a date/time to an amount");

  case value_t::AMOUNT:
    return *((amount_t *) val.data);

  case value_t::BALANCE:
    return balance_getitem(*((balance_t *) val.data), i);

  case value_t::BALANCE_PAIR:
    return balance_pair_getitem(*((balance_pair_t *) val.data), i);

  case value_t::STRING:
    throw_(value_exception, "Cannot cast a string to an amount");

  case value_t::XML_NODE:
    return (*(xml::node_t **) data)->to_value();

  case value_t::POINTER:
    throw_(value_exception, "Cannot cast a pointer to an amount");

  case value_t::SEQUENCE:
    return (*(value_t::sequence_t **) val.data)[i];

  default:
    assert(0);
    break;
  }
  assert(0);
  return 0L;
}

double py_to_float(value_t& val)
{
  return double(val);
}

void export_value()
{
  class_< value_t > ("value")
    .def(init<value_t>())
    .def(init<balance_pair_t>())
    .def(init<balance_t>())
    .def(init<amount_t>())
    .def(init<string>())
    .def(init<double>())
    .def(init<long>())
    .def(initmoment_t())

    .def(self + self)
    .def(self + other<string>())
    .def(self + other<balance_pair_t>())
    .def(self + other<balance_t>())
    .def(self + other<amount_t>())
    .def(self + long())
    .def(self + double())

    .def(other<string>() + self)
    .def(other<balance_pair_t>() + self)
    .def(other<balance_t>() + self)
    .def(other<amount_t>() + self)
    .def(long() + self)
    .def(double() + self)

    .def(self - self)
    .def(self - other<string>())
    .def(self - other<balance_pair_t>())
    .def(self - other<balance_t>())
    .def(self - other<amount_t>())
    .def(self - long())
    .def(self - double())

    .def(other<string>() - self)
    .def(other<balance_pair_t>() - self)
    .def(other<balance_t>() - self)
    .def(other<amount_t>() - self)
    .def(long() - self)
    .def(double() - self)

    .def(self * self)
    .def(self * other<string>())
    .def(self * other<balance_pair_t>())
    .def(self * other<balance_t>())
    .def(self * other<amount_t>())
    .def(self * long())
    .def(self * double())

    .def(other<string>() * self)
    .def(other<balance_pair_t>() * self)
    .def(other<balance_t>() * self)
    .def(other<amount_t>() * self)
    .def(long() * self)
    .def(double() * self)

    .def(self / self)
    .def(self / other<string>())
    .def(self / other<balance_pair_t>())
    .def(self / other<balance_t>())
    .def(self / other<amount_t>())
    .def(self / long())
    .def(self / double())

    .def(other<string>() / self)
    .def(other<balance_pair_t>() / self)
    .def(other<balance_t>() / self)
    .def(other<amount_t>() / self)
    .def(long() / self)
    .def(double() / self)

    .def(- self)

    .def(self += self)
    .def(self += other<string>())
    .def(self += other<balance_pair_t>())
    .def(self += other<balance_t>())
    .def(self += other<amount_t>())
    .def(self += long())
    .def(self += double())

    .def(self -= self)
    .def(self -= other<string>())
    .def(self -= other<balance_pair_t>())
    .def(self -= other<balance_t>())
    .def(self -= other<amount_t>())
    .def(self -= long())
    .def(self -= double())

    .def(self *= self)
    .def(self *= other<string>())
    .def(self *= other<balance_pair_t>())
    .def(self *= other<balance_t>())
    .def(self *= other<amount_t>())
    .def(self *= long())
    .def(self *= double())

    .def(self /= self)
    .def(self /= other<string>())
    .def(self /= other<balance_pair_t>())
    .def(self /= other<balance_t>())
    .def(self /= other<amount_t>())
    .def(self /= long())
    .def(self /= double())

    .def(self <  self)
    .def(self < other<string>())
    .def(self < other<balance_pair_t>())
    .def(self < other<balance_t>())
    .def(self < other<amount_t>())
    .def(self < long())
    .def(self < othermoment_t())
    .def(self < double())

    .def(other<string>() < self)
    .def(other<balance_pair_t>() < self)
    .def(other<balance_t>() < self)
    .def(other<amount_t>() < self)
    .def(long() < self)
    .def(othermoment_t() < self)
    .def(double() < self)

    .def(self <= self)
    .def(self <= other<string>())
    .def(self <= other<balance_pair_t>())
    .def(self <= other<balance_t>())
    .def(self <= other<amount_t>())
    .def(self <= long())
    .def(self <= othermoment_t())
    .def(self <= double())

    .def(other<string>() <= self)
    .def(other<balance_pair_t>() <= self)
    .def(other<balance_t>() <= self)
    .def(other<amount_t>() <= self)
    .def(long() <= self)
    .def(othermoment_t() <= self)
    .def(double() <= self)

    .def(self > self)
    .def(self > other<string>())
    .def(self > other<balance_pair_t>())
    .def(self > other<balance_t>())
    .def(self > other<amount_t>())
    .def(self > long())
    .def(self > othermoment_t())
    .def(self > double())

    .def(other<string>() > self)
    .def(other<balance_pair_t>() > self)
    .def(other<balance_t>() > self)
    .def(other<amount_t>() > self)
    .def(long() > self)
    .def(othermoment_t() > self)
    .def(double() > self)

    .def(self >= self)
    .def(self >= other<string>())
    .def(self >= other<balance_pair_t>())
    .def(self >= other<balance_t>())
    .def(self >= other<amount_t>())
    .def(self >= long())
    .def(self >= othermoment_t())
    .def(self >= double())

    .def(other<string>() >= self)
    .def(other<balance_pair_t>() >= self)
    .def(other<balance_t>() >= self)
    .def(other<amount_t>() >= self)
    .def(long() >= self)
    .def(othermoment_t() >= self)
    .def(double() >= self)

    .def(self == self)
    .def(self == other<string>())
    .def(self == other<balance_pair_t>())
    .def(self == other<balance_t>())
    .def(self == other<amount_t>())
    .def(self == long())
    .def(self == othermoment_t())
    .def(self == double())

    .def(other<string>() == self)
    .def(other<balance_pair_t>() == self)
    .def(other<balance_t>() == self)
    .def(other<amount_t>() == self)
    .def(long() == self)
    .def(othermoment_t() == self)
    .def(double() == self)

    .def(self != self)
    .def(self != other<string>())
    .def(self != other<balance_pair_t>())
    .def(self != other<balance_t>())
    .def(self != other<amount_t>())
    .def(self != long())
    .def(self != othermoment_t())
    .def(self != double())

    .def(other<string>() != self)
    .def(other<balance_pair_t>() != self)
    .def(other<balance_t>() != self)
    .def(other<amount_t>() != self)
    .def(long() != self)
    .def(othermoment_t() != self)
    .def(double() != self)

    .def(! self)

    .def(self_ns::int_(self))
    .def(self_ns::float_(self))
    .def(self_ns::str(self))

    .def_readonly("type", &value_t::type)

    .def("__abs__", &value_t::abs)
    .def("__len__", value_len)
    .def("__getitem__", value_getitem)

    .def("cast", &value_t::cast)
    .def("cost", &value_t::cost)
    .def("price", &value_t::price)
    .def("date", &value_t::date)
    .def("strip_annotations", &value_t::strip_annotations)
    .def("add", &value_t::add, return_internal_reference<>())
    .def("value", &value_t::value)
    .def("round", &value_t::round)
    .def("negate", &value_t::negate)
    .def("write", &value_t::write)
    ;

  enum_< value_t::type_t > ("ValueType")
    .value("Boolean", value_t::BOOLEAN)
    .value("Integer", value_t::INTEGER)
    .value("DateTime", value_t::DATETIME)
    .value("Amount", value_t::AMOUNT)
    .value("Balance", value_t::BALANCE)
    .value("BalancePair", value_t::BALANCE_PAIR)
    .value("String", value_t::STRING)
    .value("XmlNode", value_t::XML_NODE)
    .value("Pointer", value_t::POINTER)
    .value("Sequence", value_t::SEQUENCE)
    ;
}
