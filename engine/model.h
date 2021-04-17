namespace stacuist::engine {

class Model {
 public:
  Model(const Model&) = delete;
  Model(const Model&&) = delete;
  Model operator=(const Model&) = delete;
  Model operator=(const Model&&) = delete;

  void Do() const;

 private:
};

};  // namespace stacuist::engine
