/**
 * @file   clockreset.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Dec 15 14:11:29 2019
 *
 * @brief  clock reset action
 *
 *
 */

#ifndef CLOCK_RESET_H
#define CLOCK_RESET_H
namespace graphsat {
class ClockReset {
 public:
  ClockReset(const Argument& clock_arg, const Argument& rhs_arg)
      : clock(clock_arg), rhs(rhs_arg) {}
  void to_real(const TOReal* convertor) {
    clock_value = convertor->to_real(CLOCK_T, clock);
    rhs_value = convertor->to_real(INT_T, rhs);
  }
  void setSelectValue(const int select_value) {
    clock_value.setSelectValue(select_value);
    rhs_value.setSelectValue(select_value);
  }
  std::pair<int, int> getValue() const {
    return std::make_pair(clock_value.getValue(), rhs_value.getValue());
  }

 private:
  Argument clock, rhs;
  RealArgument clock_value, rhs_value;
};
}  // namespace graphsat

#endif
