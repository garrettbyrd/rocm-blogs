#include<hip/hip_runtime.h>

#define N 1024 * 1024 * 8
#define NITER 128

__global__ void kernel_unroll(float* in, size_t fac)
{
  size_t tid = threadIdx.x;

  if (tid >= N)
    return;

  float temp[NITER];

  // No pragma unroll results in the largest s/vgprs + lowest occupancy.
  //  - This is because compiler still defaults to unroll factor of 128
  // Unroll factor = 1 leads to lowest vgprs and better occupancy.
  // Unroll factor >=64 leads to larger vgprs and lower occupancy.
  #pragma unroll 32
  for (size_t it = 0; it < NITER; ++it)
    temp[it] = in[tid + it*fac];

  for (size_t it = 0; it < NITER; ++it)
    if (temp[it] < 0.0)
      in[tid + it*fac] = 0.0;
}
