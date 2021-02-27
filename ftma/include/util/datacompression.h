/**
 * @file   datacompression.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Jun 10 11:25:22 2019
 *
 * @brief  compression to unsign int when known the lower bound and upper bound
 * of data [ low, up]
 *
 *
 */

#ifndef __DATA_COMPRESSION_H
#define __DATA_COMPRESSION_H
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#include "util/typedef.h"
namespace graphmodel {

template <typename T>
class Compression {
 public:
  Compression() { original_data_len = 0; }
  Compression(int len)
      : original_data_len(len),
        bounds(len),
        domain(len, std::numeric_limits<UINT>::max()),
        bases(len, 1),
        shift(len, true),
        compression_index(len) {
    for (int i = 0; i < len; i++) {
      bounds[i].first = std::numeric_limits<int>::min();
      bounds[i].second = std::numeric_limits<int>::max();
    }
    update();
  }

  void setBound(int id, int low, int up) {
    assert(up >= low);
    bounds[id].first = low;
    bounds[id].second = up;

    UINT bound = up - low + 1;
    domain[id] = bound;
    update();
  }

  int getDataLen() const { return original_data_len; }

  int getCompressionSize() const { return compressionSize; }

  void encode(UINT* out, const T* const data) const {
    std::fill(out, out + compressionSize, 0);

    for (int i = 0; i < original_data_len; i++) {
      assert(data[i] >= bounds[i].first);
      assert(data[i] <= bounds[i].second);

      int j = compression_index[i];
      out[j] += (data[i] - bounds[i].first) * bases[i];
    }
  }

  void decode(T* out, const UINT* const data) const {
    int j = 0;
    UINT dummy = data[0];
    for (int i = 0; i < original_data_len; i++) {
      if (shift[i]) {
        j++;
        dummy = data[j];
      }

      out[i] = (dummy % domain[i]) + bounds[i].first;
      dummy /= domain[i];
    }
  }

 private:
  int original_data_len;
  /**
   * bounds[ i].frist  <= value[ i] <= bounds[ i].second
   *
   */

  std::vector<std::pair<T, T>> bounds;
  std::vector<UINT> domain;
  std::vector<UINT> bases;

  std::vector<bool> shift;
  std::vector<int> compression_index;
  int compressionSize;

  void update() {
    std::fill(shift.begin(), shift.end(), false);
    std::fill(bases.begin(), bases.end(), 1);
    std::fill(compression_index.begin(), compression_index.end(), 0);
    compressionSize = 1;
    UINT dummy = std::numeric_limits<UINT>::max();
    int j = 0;
    for (int i = 0; i < original_data_len; i++) {
      if (dummy < domain[i]) {
        shift[i] = true;
        dummy = std::numeric_limits<UINT>::max();
        compressionSize++;
      }
      if (shift[i]) {
        j++;
        bases[i] = 1;

      } else if (i > 0) {
        bases[i] = bases[i - 1] * domain[i - 1];
      }
      compression_index[i] = j;
      dummy /= domain[i];
    }
  }
};

template <typename C>
class StateConvert {
 public:
  StateConvert() {
    head_length = 0;
    head_comp = 0;
    compression_size = 0;
  }
  StateConvert(int hLen, int bLen, Compression<C> hCom, Compression<C> bCom) {
    head_length = hLen;

    com_head_length = hCom.getCompressionSize();

    compression_size = hCom.getCompressionSize() + bCom.getCompressionSize();
    head_comp = hCom;
    body_comp = bCom;
  }

  void encode(UINT* out, const C* data) const {
    head_comp.encode(out, data);
    body_comp.encode(out + com_head_length, data + head_length);
  }

  void decode(C* out, const UINT* data) const {
    head_comp.decode(out, data);
    body_comp.decode(out + head_length, data + com_head_length);
  }
  int getCompressionSize() const { return compression_size; }

  int getCompressionHeadSize() const { return head_comp.getCompressionSize(); }

 private:
  int head_length;
  int com_head_length;

  int compression_size;
  Compression<C> head_comp;
  Compression<C> body_comp;
};

}  // namespace graphmodel

#endif
