/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

/* SkullStripping.cxx
This is a demo program for testing the itkStripTsImageFilter for automatic skull-stripping.

version: 1.0
date: 20.04.2012
initial release

Stefan Bauer
Medical Image Analysis Group, Institute for Surgical Technology and Biomechanics, University of Bern
stefan.bauer@istb.unibe.ch
*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkMaskImageFilter.h"
#include "itkStripTsImageFilter.h"

#include "itkPluginUtilities.h"

#include "SwissSkullStripperCLP.h"

template <class T>
int DoIt( int argc, char* argv[], T )
{
  PARSE_ARGS;

  typedef    T InputPixelType;
  typedef itk::Image<InputPixelType, 3> ImageType;

  typedef itk::Image<short, 3> AtlasImageType;
  typedef itk::Image<unsigned char, 3> AtlasLabelType;


  // image reading
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileReader<AtlasImageType> AtlasReaderType;
  typedef itk::ImageFileReader<AtlasLabelType> LabelReaderType;

  typename ReaderType::Pointer patientReader = ReaderType::New();
  typename AtlasReaderType::Pointer atlasReader = AtlasReaderType::New();
  typename LabelReaderType::Pointer labelReader = LabelReaderType::New();

  patientReader->SetFileName( patientVolume );
  atlasReader->SetFileName( atlasMRIVolume );
  labelReader->SetFileName( atlasMaskVolume );

  try
    {
    patientReader->Update();
    atlasReader->Update();
    labelReader->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }

  // perform skull-stripping using stripTsImageFilter
  std::cout << std::endl << "Performing skull-stripping" << std::endl;

  // set up skull-stripping filter
  typedef itk::StripTsImageFilter<ImageType, AtlasImageType, AtlasLabelType> StripTsFilterType;
  typename StripTsFilterType::Pointer stripTsFilter = StripTsFilterType::New();

  // set the required inputs for the stripTsImageFilter
  stripTsFilter->SetInput( patientReader->GetOutput() );
  stripTsFilter->SetAtlasImage( atlasReader->GetOutput() );
  stripTsFilter->SetAtlasBrainMask( labelReader->GetOutput() );

  try
    {
    itk::PluginFilterWatcher watchStripper(stripTsFilter,
                                           "Strip skull",
                                           CLPProcessInformation);
    stripTsFilter->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }


  // mask the patient image using the output generated from the stripTsImageFilter as mask
  typedef itk::MaskImageFilter<ImageType, AtlasLabelType, ImageType> MaskFilterType;
  typename MaskFilterType::Pointer maskFilter = MaskFilterType::New();

  maskFilter->SetInput1( patientReader->GetOutput() );
  maskFilter->SetInput2( stripTsFilter->GetOutput() );

  try
    {
    maskFilter->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }


  // write mask and masked patient image
  typedef itk::ImageFileWriter<AtlasLabelType> MaskWriterType;
  typename MaskWriterType::Pointer maskWriter = MaskWriterType::New();
  maskWriter->SetInput( stripTsFilter->GetOutput() );
  maskWriter->SetFileName( patientMaskLabel );

  typedef itk::ImageFileWriter<ImageType> ImageWriterType;
  typename ImageWriterType::Pointer imageWriter = ImageWriterType::New();
  imageWriter->SetInput( maskFilter->GetOutput() );
  imageWriter->SetFileName( patientOutputVolume );

  try
    {
    maskWriter->Update();
    imageWriter->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}

int main( int argc, char * argv[] )
{
  PARSE_ARGS;

  itk::ImageIOBase::IOPixelType     pixelType;
  itk::ImageIOBase::IOComponentType componentType;

  try
    {
    itk::GetImageType(patientVolume, pixelType, componentType);

    // This filter handles all types on input, but only produces
    // signed types
    switch( componentType )
      {
#if 0
      case itk::ImageIOBase::UCHAR:
        return DoIt( argc, argv, static_cast<unsigned char>(0) );
        break;
      case itk::ImageIOBase::CHAR:
        return DoIt( argc, argv, static_cast<char>(0) );
        break;
      case itk::ImageIOBase::USHORT:
        return DoIt( argc, argv, static_cast<unsigned short>(0) );
        break;
#endif
      case itk::ImageIOBase::SHORT:
        return DoIt( argc, argv, static_cast<short>(0) );
        break;
#if 0
      case itk::ImageIOBase::UINT:
        return DoIt( argc, argv, static_cast<unsigned int>(0) );
        break;
      case itk::ImageIOBase::INT:
        return DoIt( argc, argv, static_cast<int>(0) );
        break;
      case itk::ImageIOBase::ULONG:
        return DoIt( argc, argv, static_cast<unsigned long>(0) );
        break;
      case itk::ImageIOBase::LONG:
        return DoIt( argc, argv, static_cast<long>(0) );
        break;
      case itk::ImageIOBase::FLOAT:
        return DoIt( argc, argv, static_cast<float>(0) );
        break;
      case itk::ImageIOBase::DOUBLE:
        return DoIt( argc, argv, static_cast<double>(0) );
        break;
#endif
      case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
      default:
        std::cerr << argv[0] << "unknown component type" << std::endl;
        return EXIT_FAILURE;
      }
    }

  catch( itk::ExceptionObject & excep )
    {
    std::cerr << argv[0] << ": exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
