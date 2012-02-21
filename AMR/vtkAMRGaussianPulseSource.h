/*=========================================================================

 Program:   Visualization Toolkit
 Module:    vtkAMRGaussianPulseSource.h

 Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 All rights reserved.
 See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notice for more information.

 =========================================================================*/
// .NAME vtkAMRGaussianPulseSource.h -- {Enter documentation here!}
//
// .SECTION Description
//  TODO: Enter documentation here!
#ifndef VTKAMRGAUSSIANPULSESOURCE_H_
#define VTKAMRGAUSSIANPULSESOURCE_H_

#include "vtkOverlappingAMRAlgorithm.h"

#include <cmath> // For std::exp

class vtkOverlappingAMR;
class vtkUniformGrid;
class vtkInformation;
class vtkInformationVector;

class VTK_AMR_EXPORT vtkAMRGaussianPulseSource :
  public vtkOverlappingAMRAlgorithm
{
  public:
    static vtkAMRGaussianPulseSource* New();
    vtkTypeMacro(vtkAMRGaussianPulseSource, vtkOverlappingAMRAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    // Description:
    // Sets the dimension of the AMR dataset to generate
    vtkSetMacro(Dimension,int);

    // Description:
    // Sets the number of levels to generate
    vtkSetMacro(NumberOfLevels,int);

    // Description:
    // Set the refinement ratio
    void SetRefinementRatio(int r)
     {this->RefinmentRatio=r;this->Modified();}

    // Description:
    // Set the root spacing
    void SetRootSpacing(double h0)
      {
      this->RootSpacing[0]=this->RootSpacing[1]=this->RootSpacing[2]=h0;
      this->Modified();
      }

    // Description:
    // Set & Get macro for the pulse origin
    vtkSetVector3Macro(PulseOrigin,double);
    vtkGetVector3Macro(PulseOrigin,double);
    void SetXPulseOrigin(double f)
     {this->PulseOrigin[0]=f;this->Modified();}
    void SetYPulseOrigin(double f)
     {this->PulseOrigin[1]=f;this->Modified();}
    void SetZPulseOrigin(double f)
     {this->PulseOrigin[2]=f;this->Modified();}

    // Description:
    // Set & Get macro for the pulse width
    vtkSetVector3Macro(PulseWidth,double);
    vtkGetVector3Macro(PulseWidth,double);
    void SetXPulseWidth(double f)
      {this->PulseWidth[0]=f;this->Modified();}
    void SetYPulseWidth(double f)
      {this->PulseWidth[1]=f;this->Modified();}
    void SetZPulseWidth(double f)
      {this->PulseWidth[2]=f;this->Modified();}

    // Description:
    // Set & Get macro for the pulse amplitude
    vtkSetMacro(PulseAmplitude,double);
    vtkGetMacro(PulseAmplitude,double);

  protected:
    vtkAMRGaussianPulseSource();
    virtual ~vtkAMRGaussianPulseSource();

    // Description:
    // This is called by the superclass.
    // This is the method you should override.
    virtual int RequestData(vtkInformation *request,
                            vtkInformationVector **inputVector,
                            vtkInformationVector *outputVector);

    // Description:
    // Computes the gaussian pulse at the given location based on the user
    // supplied parameters for pulse width and origin.
    double ComputePulseAt(const double x, const double y, const double z);
    double ComputePulseAt( double pt[3] )
     {return( this->ComputePulseAt(pt[0],pt[1],pt[2]) );}

    // Description:
    // Given the cell index w.r.t. to a uniform grid, this method computes the
    // cartesian coordinates of the centroid of the cell.
    void ComputeCellCenter(vtkUniformGrid *grid,
                           vtkIdType cellIdx,
                           double centroid[3] );

    // Description:
    // Generates a pulse field for the given uniform grid
    void GeneratePulseField(vtkUniformGrid *grid);

    // Description:
    // Constructs a uniform grid path with the given origin/spacing and node
    // dimensions.
    vtkUniformGrid* GetGrid( double origin[3], double h[3], int ndim[3] );

    // Description:
    // Generate 2-D or 3-D DataSet
    void Generate2DDataSet(vtkOverlappingAMR* amr);
    void Generate3DDataSet(vtkOverlappingAMR* amr);

    double RootSpacing[3];
    double PulseOrigin[3];
    double PulseWidth[3];
    double PulseAmplitude;
    int    RefinmentRatio;
    int    Dimension;
    int    NumberOfLevels;

  private:
    vtkAMRGaussianPulseSource(const vtkAMRGaussianPulseSource&); // Not implemented
    void operator=(const vtkAMRGaussianPulseSource&); // Not implemented
};

//==============================================================================
// INLINE METHODS
//==============================================================================
inline double vtkAMRGaussianPulseSource::ComputePulseAt(
    const double x, const double y, const double vtkNotUsed(z))
{
  double pulse = 0.0;

  double r  = 0.0;
  double dx = x-this->PulseOrigin[0];
  r += (dx*dx) / (this->PulseWidth[0]*this->PulseWidth[0]);

  double dy = y-this->PulseOrigin[1];
  r += (dy*dy) / (this->PulseWidth[1]*this->PulseWidth[1]);

  pulse = this->PulseAmplitude*std::exp( -r );
  return( pulse );
}

#endif /* VTKAMRGAUSSIANPULSESOURCE_H_ */
