cmake_minimum_required(VERSION 2.8.9)

project(SwissSkullStripper)

#-----------------------------------------------------------------------------
set(EXTENSION_HOMEPAGE "http://www.slicer.org/slicerWiki/index.php/Documentation/Nightly/Extensions/SwissSkullStripper")
set(EXTENSION_CATEGORY "Segmentation")
set(EXTENSION_CONTRIBUTORS "Bill Lorensen (Noware), Stefan Bauer (University of Bern), Thomas Fejes (University of Bern), Mauricio Reyes (University of Bern), Arnaud Gelas (Crisalix)")
set(EXTENSION_DESCRIPTION "A skull-stripping filter that registers an atlas image to the patient data. An atlas brain mask is propagated to the patient data. This brain mask is eroded and then serves as initialization for a refined brain extraction based on level-sets. The level-set is evolved towards the edge of the brain-skull border with dedicated expansion, curvature and advection terms.")
set(EXTENSION_ICONURL "https://github.com/lorensen/SwissSkullStripperExtension/blob/master/SwissSkullStripper.pnghttps://github.com/lorensen/SwissSkullStripperExtension/blob/master/SwissSkullStripper.png")
set(EXTENSION_SCREENSHOTURLS "http://wiki.slicer.org/slicerWiki/images/a/ab/Slicer-r19441-SwissSkullStripper-screenshot.png http://wiki.slicer.org/slicerWiki/images/1/1e/Slicer-r19441-SwissSkullStripper-screenshot-2.png")
set(EXTENSION_STATUS "work-in-progress")

#-----------------------------------------------------------------------------
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
add_subdirectory(SwissSkullStripper)

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
