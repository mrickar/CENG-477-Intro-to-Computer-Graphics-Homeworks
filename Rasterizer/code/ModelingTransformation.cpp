#include "ModelingTransformation.h"

std::vector<Matrix4 *> getModelingTrasformationMatricies(std::vector<Mesh *> *meshes, std::vector<Scaling *> *scalings, std::vector<Translation *> *translations, std::vector<Rotation *> *rotations)
{
    std::vector<Matrix4 *> transformationMatricies;

    for (int mesh_idx = 0 ; mesh_idx < meshes->size() ; mesh_idx++)
	{
		Mesh *mesh = (*meshes)[mesh_idx];
		Matrix4 *transformationMatrix = new Matrix4(getIdentityMatrix());

		// compute the transformation matrix for current mesh
		for (int transformation_idx = 0 ; transformation_idx < mesh->numberOfTransformations ; transformation_idx++)
		{
			if (mesh->transformationTypes[transformation_idx] == 'r')
			{
				int rotationId = mesh->transformationIds[transformation_idx];
				double angle = (*rotations)[rotationId - 1]->angle;
				double ux = (*rotations)[rotationId - 1]->ux;
				double uy = (*rotations)[rotationId - 1]->uy;
				double uz = (*rotations)[rotationId - 1]->uz;

				Rotation *rotation = new Rotation(rotationId, angle, ux, uy, uz);

				Matrix4 matrix = rotation->getMatrix();

				*transformationMatrix = multiplyMatrixWithMatrix(matrix, *transformationMatrix);
				
				continue;
			}

			if (mesh->transformationTypes[transformation_idx] == 's')
			{
				int scalingId = mesh->transformationIds[transformation_idx];
				double sx = (*scalings)[scalingId - 1]->sx;
				double sy = (*scalings)[scalingId - 1]->sy;
				double sz = (*scalings)[scalingId - 1]->sz;

				Scaling *scaling = new Scaling(scalingId, sx, sy, sz);

				Matrix4 matrix = scaling->getMatrix();

				*transformationMatrix = multiplyMatrixWithMatrix(matrix, *transformationMatrix);

				continue;
			}

			if (mesh->transformationTypes[transformation_idx] == 't')
			{
				int translationId = mesh->transformationIds[transformation_idx];
				double tx = (*translations)[translationId - 1]->tx;
				double ty = (*translations)[translationId - 1]->ty;
				double tz = (*translations)[translationId - 1]->tz;

				Translation *translation = new Translation(translationId, tx, ty, tz);

				Matrix4 matrix = translation->getMatrix();

				*transformationMatrix = multiplyMatrixWithMatrix(matrix, *transformationMatrix);

				continue;
			}
		}

        transformationMatricies.push_back(transformationMatrix);
	}

    return transformationMatricies;
}