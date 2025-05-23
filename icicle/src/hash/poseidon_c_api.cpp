#include <memory>
#include "icicle/hash/hash.h"
#include "icicle/hash/poseidon.h"
#include "icicle/fields/field_config.h"
#include "icicle/utils/utils.h"

using namespace field_config;
using namespace icicle;

extern "C" {
typedef icicle::Hash* HasherHandle;

HasherHandle
CONCAT_EXPAND(ICICLE_FFI_PREFIX, create_poseidon_hasher)(unsigned t, const scalar_t* domain_tag, unsigned input_size)
{
  return new icicle::Hash(icicle::create_poseidon_hash<scalar_t>(t, domain_tag, input_size));
}
}