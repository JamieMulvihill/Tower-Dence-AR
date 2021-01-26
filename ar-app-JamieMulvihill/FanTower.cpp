#include "FanTower.h"
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <math.h>



void FanTower::Init(gef::Mesh* mesh, EnemyManager* enemyManager_)
{
	gef::Sphere bounds(gef::Vector4(0.0f, 0.0f, 0.0f), 2.f);
	mesh->set_bounding_sphere(bounds);
	set_mesh(mesh);

	//enemyManager = enemyManager_;

	targetRange = .1;
}

void FanTower::Update(float frametime)
{

}

void FanTower::CheckRanges(gef::Matrix44 enemyLocalTransform, gef::Matrix44 marker1transform, gef::Matrix44 inverseTowerMarker, gef::Matrix44 localMarkerTransform, PrimitiveBuilder* primitive_Builder, gef::Mesh* mesh, EnemyManager* enemyManager_)
{
	/*local_marker_transform = localMarkerTransform;
	marker_Transform = marker1transform;

	for (int i = 0; i < enemyManager_->enemies.size(); i++) {
		if (enemyManager_->enemies[i]->GetAlive()) {

			targetVector = (enemyManager_->enemies[i]->local_transform * marker1transform * inverseTowerMarker).GetTranslation() - position_;

			if (targetVector.Length() < targetRange) {
				enemyManager_->enemies[i]->SetSpeed(.05f);
			}
			else {
				enemyManager_->enemies[i]->SetSpeed(.1f);
			}
		}
*/

		/*for (std::list<Enemy*>::iterator it = enemyManager_->enemies.begin(); it != enemyManager_->enemies.end(); ++it) {
			if ((*it)->GetAlive())
			{
				Enemy *current = *it;
				targetVector = (current->local_transform * marker1transform * inverseTowerMarker).GetTranslation() - position_;

				if (targetVector.Length() < targetRange) {
					current->SetSpeed(.05f);
				}
				else {
					current->SetSpeed(.1f);
				}
			}
		}*/
	//}
}
