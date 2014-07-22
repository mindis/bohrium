from PIL import Image
import util
if util.Benchmark().bohrium:
    import bohrium as np
else:
    import numpy as np
from bohrium.stdviews import cartesian

def gen_3d_filter(size, weight, datatype=np.float32):
    """Return a (size X size X size) filter. TODO: Elaborate on this..."""

    kernel      = np.zeros((size,size,size), dtype=datatype)
    totalsum    = 0
    kernelrad   = size/2
    distance    = 0
    caleuler    = 1.0 /(2.0 * np.pi * weight**2)

    for filterZ in range(-kernelrad, kernelrad+1, 1): 
        for filterY in range(-kernelrad, kernelrad+1, 1): 
            for filterX in range(-kernelrad, kernelrad+1, 1): 
                distance = ((filterX * filterX)+(filterY*filterY)+(filterZ*filterZ))/(2 * (weight * weight)) 
                kernel[filterZ + kernelrad, filterY + kernelrad,filterX + kernelrad] = caleuler * np.exp(-distance) 
                totalsum += kernel[filterZ + kernelrad, filterY + kernelrad, filterX + kernelrad] 
    kernel *=(1.0/totalsum)

    return kernel

def convolve_3d_init(fsize, datatype=np.float32):
    """TODO: Describe the data loaded/generated by this function."""

    rgb     = np.array(np.random.random((512, 512, 512)), datatype)
    kernel  = gen_3d_filter(fsize, 13.0)

    return rgb, kernel

def convolve_3d(image, image_filter):
    """TODO: And what do we have here?"""

    views = cartesian(image, len(image_filter))
    return sum(d[0]*d[1] for d in zip(views, image_filter.flatten()))

def main():
    """
    Example parameter: --size=25.
    This will execute on a something related to 25....
    """
    B = util.Benchmark()        # Benchmark setup
    (N,) = B.size
    image, image_filter = convolve_3d_init(N)
    image + 1, image_filter + 1 # Ensure arrays are in the correct space.

    B.start()                   # Benchmark run / timing
    result = convolve_3d(image, image_filter)
    B.stop()
    B.pprint()

if __name__ == "__main__":
    main()
