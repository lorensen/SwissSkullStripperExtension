SwissSkullStripperExtension
===========================

Slice4 extension for the Swiss Skull Stripper.
The algorithm registers a grayscale atlas image to the graysacle
patient data. The associated atlas brain mask is propagated to the
patient data using the registration transformation. This brain mask is
eroded and serves as initialization for a refined brain extraction
based on level-sets. The level-set towards the edge of the brain-skull
border with dedicated expansion, curvature and advection terms.

