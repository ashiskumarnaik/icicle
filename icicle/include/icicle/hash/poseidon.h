#pragma once

#include "icicle/hash/hash.h"

namespace icicle {
  /**
   * @brief Creates a Poseidon hash instance with the specified width and optional domain tag.
   *
   * This function generates a Poseidon hash with customizable parameters to suit various cryptographic
   * contexts and use cases. The width parameter (`t`) determines the number of elements in the state,
   * influencing the security level and output structure of the hash. The optional `domain_tag` pointer parameter
   * enables domain separation, allowing isolation of hash outputs across different contexts or applications.
   * (See here for a detailed explanation:
   * https://hackmd.io/@7dpNYqjKQGeYC7wMlPxHtQ/ByIbpfX9c#SAFE-Sponge-API-for-Field-Elements-–-A-Toolbox-for-ZK-Hash-Applications)
   *
   * @param S Represents the type of the field element used by the hash (e.g., a field element class).
   *
   * @param t The width of the Poseidon hash state, representing the number of elements in the hash state.
   *          Ensure that the selected `t` is compatible with the Poseidon implementation
   *          and use case. Currently supported values are 3, 5, 9, or 12.
   *
   * @param domain_tag A pointer to an optional domain tag of type `S`. If provided, this tag is used for
   *                   domain separation, isolating hash outputs across different contexts. Passing `nullptr`
   *                   indicates that no domain separation is required, which may be sufficient for general use
   *                   cases where isolation between domains is not needed.
   *
   * @return Hash An instance of the Poseidon hash initialized with the specified parameters, ready
   *         for hashing operations.
   *
   * @note The value of `t` should align with the desired security and performance requirements of
   *       the application. The function may throw an exception or produce undefined behavior if an
   *       unsupported value for `t` is provided.
   */
  template <typename S>
  Hash create_poseidon_hash(unsigned t, const S* domain_tag = nullptr, unsigned input_size = 0);

  // Poseidon struct providing a static interface to Poseidon-related operations.
  struct Poseidon {
    template <typename S>
    inline static Hash create(unsigned t, const S* domain_tag = nullptr, unsigned input_size = 0)
    {
      return create_poseidon_hash<S>(t, domain_tag, input_size);
    }
  };

} // namespace icicle