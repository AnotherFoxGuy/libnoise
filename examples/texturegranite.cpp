// texturegranite.cpp
//
// This program uses the libnoise library to generate texture maps consisting
// of granite.
//
// Copyright (C) 2004 Jason Bevins
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// (COPYING.txt) for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include <noise.h>

#include "noiseutils.h"

using namespace noise;

// Height of the texture.
const int TEXTURE_HEIGHT = 256;

// Creates the color gradients for the texture.
void CreateTextureColor (utils::RendererImage& renderer);

// Given a noise module, this function renders a flat square texture map and
// writes it to a Windows bitmap (*.bmp) file.  Because the texture map is
// square, its width is equal to its height.  The texture map can be seamless
// (tileable) or non-seamless.
void CreatePlanarTexture (const module::Module& noiseModule, bool seamless,
  int height, const char* filename);

// Given a noise module, this function renders a spherical texture map and
// writes it to a Windows bitmap (*.bmp) file.  The texture map's width is
// double its height.
void CreateSphericalTexture (const module::Module& noiseModule, int height,
  const char* filename);

// Given a noise map, this function renders a texture map and writes it to a
// Windows bitmap (*.bmp) file.
void RenderTexture (const utils::NoiseMap& noiseMap,
  const char* filename);

int main ()
{
  // Primary granite texture.  This generates the "roughness" of the texture
  // when lit by a light source.
  module::Billow primaryGranite;
  primaryGranite.SetSeed (0);
  primaryGranite.SetFrequency (8.0);
  primaryGranite.SetPersistence (0.625);
  primaryGranite.SetLacunarity (2.18359375);
  primaryGranite.SetOctaveCount (6);
  primaryGranite.SetNoiseQuality (QUALITY_STD);

  // Use Voronoi polygons to produce the small grains for the granite texture.
  module::Voronoi baseGrains;
  baseGrains.SetSeed (1);
  baseGrains.SetFrequency (16.0);
  baseGrains.EnableDistance (true);

  // Scale the small grain values so that they may be added to the base
  // granite texture.  Voronoi polygons normally generate pits, so apply a
  // negative scaling factor to produce bumps instead.
  module::ScaleBias scaledGrains;
  scaledGrains.SetSourceModule (0, baseGrains);
  scaledGrains.SetScale (-0.5);
  scaledGrains.SetBias (0.0);

  // Combine the primary granite texture with the small grain texture.
  module::Add combinedGranite;
  combinedGranite.SetSourceModule (0, primaryGranite);
  combinedGranite.SetSourceModule (1, scaledGrains);

  // Finally, perturb the granite texture to add realism.
  module::Turbulence finalGranite;
  finalGranite.SetSourceModule (0, combinedGranite);
  finalGranite.SetSeed (2);
  finalGranite.SetFrequency (4.0);
  finalGranite.SetPower (1.0 / 8.0);
  finalGranite.SetRoughness (6);

  // Given the granite noise module, create a non-seamless texture map, a
  // seamless texture map, and a spherical texture map.
  CreatePlanarTexture (finalGranite, false, TEXTURE_HEIGHT,
    "textureplane.bmp");
  CreatePlanarTexture (finalGranite, true, TEXTURE_HEIGHT,
    "textureseamless.bmp");
  CreateSphericalTexture (finalGranite, TEXTURE_HEIGHT,
    "texturesphere.bmp");

  return 0;
}

void CreateTextureColor (utils::RendererImage& renderer)
{
  // Create a gray granite palette.  Black and pink appear at either ends of
  // the palette; those colors provide the charactistic black and pink flecks
  // in granite.
  renderer.ClearGradient ();
  renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0,   0, 255));
  renderer.AddGradientPoint (-0.9375, utils::Color (  0,   0,   0, 255));
  renderer.AddGradientPoint (-0.8750, utils::Color (216, 216, 242, 255));
  renderer.AddGradientPoint ( 0.0000, utils::Color (191, 191, 191, 255));
  renderer.AddGradientPoint ( 0.5000, utils::Color (210, 116, 125, 255));
  renderer.AddGradientPoint ( 0.7500, utils::Color (210, 113,  98, 255));
  renderer.AddGradientPoint ( 1.0000, utils::Color (255, 176, 192, 255));
}

void CreatePlanarTexture (const module::Module& noiseModule, bool seamless,
  int height, const char* filename)
{
  // Map the output values from the noise module onto a plane.  This will
  // create a two-dimensional noise map which can be rendered as a flat
  // texture map.
  utils::NoiseMapBuilderPlane plane;
  utils::NoiseMap noiseMap;
  plane.SetBounds (-1.0, 1.0, -1.0, 1.0);
  plane.SetDestSize (height, height);
  plane.SetSourceModule (noiseModule);
  plane.SetDestNoiseMap (noiseMap);
  plane.EnableSeamless (seamless);
  plane.Build ();

  RenderTexture (noiseMap, filename);
}

void CreateSphericalTexture (const module::Module& noiseModule, int height,
  const char* filename)
{
  // Map the output values from the noise module onto a sphere.  This will
  // create a two-dimensional noise map which can be rendered as a spherical
  // texture map.
  utils::NoiseMapBuilderSphere sphere;
  utils::NoiseMap noiseMap;
  sphere.SetBounds (-90.0, 90.0, -180.0, 180.0); // degrees
  sphere.SetDestSize (height * 2, height);
  sphere.SetSourceModule (noiseModule);
  sphere.SetDestNoiseMap (noiseMap);
  sphere.Build ();

  RenderTexture (noiseMap, filename);
}

void RenderTexture (const utils::NoiseMap& noiseMap, const char* filename)
{
  // Create the color gradients for the texture.
  utils::RendererImage textureRenderer;
  CreateTextureColor (textureRenderer);

  // Set up us the texture renderer and pass the noise map to it.
  utils::Image destTexture;
  textureRenderer.SetSourceNoiseMap (noiseMap);
  textureRenderer.SetDestImage (destTexture);
  textureRenderer.EnableLight (true);
  textureRenderer.SetLightAzimuth (135.0);
  textureRenderer.SetLightElev (60.0);
  textureRenderer.SetLightContrast (2.0);
  textureRenderer.SetLightColor (utils::Color (255, 255, 255, 0));

  // Render the texture.
  textureRenderer.Render ();

  // Write the texture as a Windows bitmap file (*.bmp).
  utils::WriterBMP textureWriter;
  textureWriter.SetSourceImage (destTexture);
  textureWriter.SetDestFilename (filename);
  textureWriter.WriteDestFile ();
}
