class Fault {
  private:
    bool suppressed;
    bool signaled;
    bool base;
  public:
    Fault();
    void signal();
    void release();
    void suppress();
    void unsuppress();
    


}