#include <fstream>
#include <float.h>
#include <map>
#include <omp.h>

#include <PolyMesh/PolyMesh.h>
#include <PolyMesh/util.h>

#include <GL/glut.h>

std::vector<PolyMesh *> PolyMesh::Meshes = std::vector<PolyMesh *>();
uint64_t PolyMesh::Time = 0;

PolyMesh::PolyMesh() : max_x(FLT_MIN), min_x(FLT_MAX), max_y(FLT_MIN), min_y(FLT_MAX), max_z(FLT_MIN), min_z(FLT_MAX), DrawMode(GL_TRIANGLES), ShadeMode(GL_SMOOTH), MaterialFaceMode(GL_FRONT_AND_BACK), Lighting(false), Animated(false)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
	glPopMatrix();
	Meshes.push_back(this);
}

PolyMesh::PolyMesh(std::string FilePath) : max_x(FLT_MIN), min_x(FLT_MAX), max_y(FLT_MIN), min_y(FLT_MAX), max_z(FLT_MIN), min_z(FLT_MAX), DrawMode(GL_TRIANGLES), ShadeMode(GL_SMOOTH), MaterialFaceMode(GL_FRONT_AND_BACK), Lighting(false), Animated(false)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
	glPopMatrix();
	LoadObj(FilePath);
	GenerateNormals();
	Meshes.push_back(this);
}

PolyMesh::PolyMesh(std::string FilePath, GLenum DrawMode) : max_x(FLT_MIN), min_x(FLT_MAX), max_y(FLT_MIN), min_y(FLT_MAX), max_z(FLT_MIN), min_z(FLT_MAX), DrawMode(DrawMode), ShadeMode(GL_SMOOTH), MaterialFaceMode(GL_FRONT_AND_BACK), Lighting(false), Animated(false)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
	glPopMatrix();
	LoadObj(FilePath);
	GenerateNormals();
	Meshes.push_back(this);
}

PolyMesh::PolyMesh(std::string FilePath, GLenum DrawMode, GLenum ShadeMode) : max_x(FLT_MIN), min_x(FLT_MAX), max_y(FLT_MIN), min_y(FLT_MAX), max_z(FLT_MIN), min_z(FLT_MAX), DrawMode(DrawMode), ShadeMode(ShadeMode), MaterialFaceMode(GL_FRONT_AND_BACK), Lighting(false), Animated(false)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
	glPopMatrix();
	LoadObj(FilePath);
	GenerateNormals();
	Meshes.push_back(this);
}

PolyMesh *PolyMesh::ApplyTexture(const unsigned char *data, int width, int height)
{
	Texture = data;
	TextureWidth = width;
	TextureHeight = height;
	return this;
}

PolyMesh *PolyMesh::EnableLighting()
{
	Lighting = true;
	return this;
}

PolyMesh *PolyMesh::Scale(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(x, y, z);
	glMultMatrixf(ModelView);
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
	glPopMatrix();
	return this;
}

PolyMesh *PolyMesh::Normalize()
{
	float max_dim = std::max(max_x - min_x, std::max(max_y - min_y, max_z - min_z));
	return Scale(1/max_dim, 1/max_dim, 1/max_dim);
}

PolyMesh *PolyMesh::Translate(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, z);
	glMultMatrixf(ModelView);
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
	glPopMatrix();
	return this;
}

PolyMesh *PolyMesh::Center()
{
	float cen_x, cen_y, cen_z;
	cen_x = (max_x + min_x)/2;
	cen_y = (max_y + min_y)/2;
	cen_z = (max_z + min_z)/2;
	return Translate(-cen_x, -cen_y, -cen_z);
}

PolyMesh *PolyMesh::Rotate(float angle, float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angle, x, y, z);
	glMultMatrixf(ModelView);
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
	glPopMatrix();
	return this;
}

PolyMesh *PolyMesh::GenerateNormals()
{
	int vertices = n_vertices();

	// Request normals
	this->request_vertex_normals();
	this->request_face_normals();

	// Generate vertex normals
#pragma omp parallel for default(none) shared(vertices)
	for(int i = 0; i < vertices; i++)
	{
		VertexHandle handle = vertex_handle(i);

		Normal normal;
		normal[0] = normal[1] = normal[2] = 0.0f;

		// Compute face normals and sum
		for (VertexFaceIter vf_it = this->vf_begin(handle); vf_it != this->vf_end(handle); ++vf_it)
		{
			this->set_normal(vf_it.handle(), this->calc_face_normal(vf_it.handle()));

			normal += this->calc_face_normal(vf_it.handle());
		}

		// Normalize vertex normal
		normal.normalize();

		// Set normal
		this->set_normal(handle, normal);
	}

	return this;
}

PolyMesh *PolyMesh::LoopSubdivideP(int numThreads)
{
	// Set number of threads to use	
	omp_set_num_threads(numThreads);

	// Mesh counts
	int edges = n_edges();
	int vertices = n_vertices();
	int faces = n_faces();

	// New mesh data
	LoopVertex *newEvenVertices = (LoopVertex *)calloc(vertices, sizeof (LoopVertex));
	LoopVertex *newOddVertices = (LoopVertex *)calloc(edges, sizeof (LoopVertex));
	LoopFace *newFaces = (LoopFace *)calloc(faces*4, sizeof(LoopFace));

#pragma omp parallel default(none) shared(edges, vertices, faces, newEvenVertices, newOddVertices, newFaces)
	{
		// Create an odd vertex on every edge
#pragma omp for nowait
		for (int i = 0; i < edges; i++)
		{
			EdgeHandle handle = edge_handle(i);

			// Obtain half edges
			HalfedgeHandle HalfEdges[2];
			HalfEdges[0] = halfedge_handle(handle, 0);
			HalfEdges[1] = halfedge_handle(handle, 1);

			// Obtain faces associated with halfedges
			FaceHandle Faces[2];
			Faces[0] = face_handle(HalfEdges[0]);
			Faces[1] = face_handle(HalfEdges[1]);

			// Obtain vertices bounding the edge
			VertexHandle Vertices[4];
			Vertices[0] = to_vertex_handle(HalfEdges[0]);
			Vertices[1] = from_vertex_handle(HalfEdges[0]);

			// If first face is valid, obtain handle of opposite edge. No need to
			// parallelize this - only three iterations. Creating the threads will
			// probably be a larger overhead.
			if(Faces[0].is_valid())
			{
				for (FaceVertexIter fv_it = fv_begin(Faces[0]); fv_it != fv_end(Faces[0]); ++fv_it)
				{
					if (Vertices[0] != fv_it.handle() && Vertices[1] != fv_it.handle())
						Vertices[2] = fv_it.handle();
				}
			}

			// If second face is valid, obtain handle of opposite edge. No need to
			// parallelize here either.
			if(Faces[1].is_valid()) {
				for (FaceVertexIter fv_it = fv_begin(Faces[1]); fv_it != fv_end(Faces[1]); ++fv_it)
				{
					if (Vertices[0] != fv_it.handle() && Vertices[1] != fv_it.handle())
						Vertices[3] = fv_it.handle();
				}
			}

			// Compute odd vertex
			Point OddVertex;
			if (Faces[0].is_valid() && Faces[1].is_valid())
			{
				// Both faces exist: vertex is not on boundary
				OddVertex = (point(Vertices[2]) + point(Vertices[3])) / 8.0 + (point(Vertices[0]) + point(Vertices[1])) * (3.0 / 8.0);

				// Add odd vertex to graph
				newOddVertices[handle.idx()].point = OddVertex;

				// Compute new texture coordinates
				if (has_vertex_texcoords2D())
				{
					TexCoord2D NewTexCoord = (texcoord2D(Vertices[2]) + texcoord2D(Vertices[3])) / 8.0 + (texcoord2D(Vertices[0]) + texcoord2D(Vertices[1])) * (3.0 / 8.0);
					newOddVertices[handle.idx()].tex = NewTexCoord;
				}
			}
			else
			{
				// A face is missing: vertex is on boundary
				OddVertex = (point(Vertices[0]) + point(Vertices[1])) / 2.0;

				// Add odd vertex to graph
				newOddVertices[handle.idx()].point = OddVertex;

				// Compute new texture coordinates
				if (has_vertex_texcoords2D())
				{
					TexCoord2D NewTexCoord = (texcoord2D(Vertices[0]) + texcoord2D(Vertices[1])) / 2.0;
					newOddVertices[handle.idx()].tex = NewTexCoord;
				}
			}
		}

		// Record 3 odd and 3 even vertices of each face
#pragma omp for nowait
		for(int i = 0; i < faces; i++)
		{
			FaceHandle handle = face_handle(i);

			// Obtain edges of face
			EdgeHandle OldEdges [3];
			FaceEdgeIter fe_it = fe_begin(handle);
			OldEdges[0] = fe_it.handle();
			OldEdges[1] = (++fe_it).handle();
			OldEdges[2] = (++fe_it).handle();

			// Obtain odd vertices from edges
			int OddVertices[3];
			OddVertices[0] = OldEdges[0].idx();
			OddVertices[1] = OldEdges[1].idx();
			OddVertices[2] = OldEdges[2].idx();

			// Obtain even vertices
			int EvenVertices[3];
			FaceVertexIter fv_it = fv_begin(handle);
			EvenVertices[0] = fv_it.handle().idx();
			EvenVertices[1] = (++fv_it).handle().idx();
			EvenVertices[2] = (++fv_it).handle().idx();

			newFaces[i].a = EvenVertices[0];
			newFaces[i].b = EvenVertices[1];
			newFaces[i].c = EvenVertices[2];
			newFaces[i].d = OddVertices[0];
			newFaces[i].e = OddVertices[1];
			newFaces[i].f = OddVertices[2];
		}

		// Apply even vertex mask
#pragma omp for nowait
		for (int i = 0; i < vertices; i++)
		{
			VertexHandle handle = vertex_handle(i);
			// Vertex is on boundary: apply special mask
			if (is_boundary(handle))
			{
				Point NeighborPointSum = point(handle) * 3.0 / 4.0;
				TexCoord2D NeighborTexCoordSum;
				if (has_vertex_texcoords2D())
					NeighborTexCoordSum = texcoord2D(handle) * 3.0 / 4.0;

				for (VertexEdgeIter ve_it = ve_begin(handle); ve_it != ve_end(handle); ++ve_it)
				{
					if (is_boundary(ve_it.handle()))
					{
						VertexHandle otherPoint = to_vertex_handle(halfedge_handle(ve_it.handle(), 0));
						if (point(otherPoint) == point(handle))
							otherPoint = from_vertex_handle(halfedge_handle(ve_it.handle(), 0));
						NeighborPointSum += point(otherPoint) / 8.0;
						if (has_vertex_texcoords2D())
							NeighborTexCoordSum += texcoord2D(otherPoint) / 8.0;
					}
				}

				newEvenVertices[handle.idx()].point = NeighborPointSum;
				if (has_vertex_texcoords2D())
					newEvenVertices[handle.idx()].tex = NeighborTexCoordSum;
			}
			else
			{
				// Vertex is not on boundary: apply normal mask
				int NeighborCount = 0;
				Point NeighborPointSum;
				NeighborPointSum[0] = NeighborPointSum[1] = NeighborPointSum[2] = 0.0f;
				TexCoord2D NeighborTexCoordSum;
				if (has_vertex_texcoords2D())
					NeighborTexCoordSum[0] = NeighborTexCoordSum[1] = 0.0f;

				// No need to parallelize - rank is probably not very large.
				for (VertexVertexIter vv_it = vv_begin(handle); vv_it != vv_end(handle); ++vv_it)
				{
					NeighborPointSum += point(vv_it.handle());
					if(has_vertex_texcoords2D())
						NeighborTexCoordSum += texcoord2D(vv_it.handle());
					NeighborCount++;
				}

				float B;

				if(NeighborCount > 3)
					B = 3.0f / (8.0f * NeighborCount);
				else
					B = 3.0f / 16.0f;

				NeighborPointSum = NeighborPointSum * B + (1 - NeighborCount * B) * point(handle);
				if (has_vertex_texcoords2D())
					NeighborTexCoordSum = NeighborTexCoordSum * B + (1 - NeighborCount * B) * texcoord2D(handle);

				newEvenVertices[handle.idx()].point = NeighborPointSum;
				if (has_vertex_texcoords2D())
					newEvenVertices[handle.idx()].tex = NeighborTexCoordSum;
			}
		}
	}

	clear();

	// Add new even vertices to the mesh. Can't be parallelized.
	for (int i = 0; i < vertices; i++)
	{
		newEvenVertices[i].newVH = add_vertex(newEvenVertices[i].point);
		if (has_vertex_texcoords2D())
			set_texcoord2D(newEvenVertices[i].newVH, newEvenVertices[i].tex);
	}
	// Add new odd vertices to the mesh. Can't be parallelized.
	for (int i = 0; i < edges; i++)
	{
		newOddVertices[i].newVH = add_vertex(newOddVertices[i].point);
		if (has_vertex_texcoords2D())
			set_texcoord2D(newOddVertices[i].newVH, newOddVertices[i].tex);
	}

	// Add new faces to mesh. Can't be parallelized
	for(int i = 0; i < faces; i++)
	{
		LoopFace face = newFaces[i];
		//
		//             face[2]
		//                ^
		//               / \
		//              /   \
		//     face[3] o     o face[5] 
		//            /       \
		//           /____o____\
		//    face[0]  face[4]  face[1]
		//
		add_face(newOddVertices[face.d].newVH, newEvenVertices[face.a].newVH, newOddVertices[face.e].newVH);
		add_face(newEvenVertices[face.c].newVH, newOddVertices[face.d].newVH, newOddVertices[face.f].newVH);
		add_face(newOddVertices[face.f].newVH, newOddVertices[face.e].newVH, newEvenVertices[face.b].newVH);
		add_face(newOddVertices[face.d].newVH, newOddVertices[face.e].newVH, newOddVertices[face.f].newVH);
	}

	// Regenerate normals
	GenerateNormals();

	free(newEvenVertices);
	free(newOddVertices);
	free(newFaces);

	return this;
}

PolyMesh *PolyMesh::LoopSubdivide()
{
	// Maintain lists of even and odd vertices
	std::vector<VertexHandle> EvenVertices;
	std::map<EdgeHandle, VertexHandle> OddVertices;

	// Mark existing vertices as even
	for (VertexIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		EvenVertices.push_back(v_it.handle());
	}

	// Create an odd vertex on every edge
	for (EdgeIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		// Obtain half edges
		HalfedgeHandle HalfEdges[2];
		HalfEdges[0] = this->halfedge_handle(e_it.handle(), 0);
		HalfEdges[1] = this->halfedge_handle(e_it.handle(), 1);

		// Obtain faces associated with halfedges
		FaceHandle Faces[2];
		Faces[0] = this->face_handle(HalfEdges[0]);
		Faces[1] = this->face_handle(HalfEdges[1]);

		// Obtain vertices bounding the edge
		VertexHandle Vertices[4];
		Vertices[0] = this->to_vertex_handle(HalfEdges[0]);
		Vertices[1] = this->from_vertex_handle(HalfEdges[0]);

		// If first face is valid, obtain handle of opposite edge
		if(Faces[0].is_valid())
		{
			for (FaceVertexIter fv_it = fv_begin(Faces[0]); fv_it != fv_end(Faces[0]); ++fv_it)
			{
				if (Vertices[0] != fv_it.handle() && Vertices[1] != fv_it.handle())
					Vertices[2] = fv_it.handle();
			}
		}

		// If second face is valid, obtain handle of opposite edge
		if(Faces[1].is_valid()) {
			for (FaceVertexIter fv_it = fv_begin(Faces[1]); fv_it != fv_end(Faces[1]); ++fv_it)
			{
				if (Vertices[0] != fv_it.handle() && Vertices[1] != fv_it.handle())
					Vertices[3] = fv_it.handle();
			}
		}

		// Compute odd vertex
		Point OddVertex;
		if (Faces[0].is_valid() && Faces[1].is_valid())
		{
			// Both faces exist: vertex is not on boundary
			OddVertex = (point(Vertices[2]) + point(Vertices[3])) / 8.0 + (point(Vertices[0]) + point(Vertices[1])) * (3.0 / 8.0);

			// Add odd vertex to graph
			VertexHandle OddVertexHandle = add_vertex(OddVertex);

			// Associate odd vertex with the edge it lies on
			OddVertices.insert(std::pair<EdgeHandle, VertexHandle>(e_it.handle(), OddVertexHandle));

			// Compute new texture coordinates
			if (has_vertex_texcoords2D())
			{
				TexCoord2D NewTexCoord = (texcoord2D(Vertices[2]) + texcoord2D(Vertices[3])) / 8.0 + (texcoord2D(Vertices[0]) + texcoord2D(Vertices[1])) * (3.0 / 8.0);
				set_texcoord2D(OddVertexHandle, NewTexCoord);
			}
		}
		else
		{
			// A face is missing: vertex is on boundary
			OddVertex = (point(Vertices[0]) + point(Vertices[1])) / 2.0;

			// Add odd vertex to graph
			VertexHandle OddVertexHandle = add_vertex(OddVertex);

			// Associate odd vertex with the edge it lies on
			OddVertices.insert(std::pair<EdgeHandle, VertexHandle>(e_it.handle(), OddVertexHandle));

			// Compute new texture coordinates
			if (has_vertex_texcoords2D())
			{
				TexCoord2D NewTexCoord = (texcoord2D(Vertices[0]) + texcoord2D(Vertices[1])) / 2.0;
				set_texcoord2D(OddVertexHandle, NewTexCoord);
			}
		}
	}

	// Record 3 odd and 3 even vertices of each face
	std::vector<std::vector<VertexHandle>> VertexCollections;

	for(FaceIter f_it = faces_begin(); f_it != faces_end(); ++f_it)
	{
		// Obtain edges of face
		EdgeHandle OldEdges [3];
		FaceEdgeIter fe_it = fe_begin(f_it.handle());
		OldEdges[0] = fe_it.handle();
		OldEdges[1] = (++fe_it).handle();
		OldEdges[2] = (++fe_it).handle();

		// Obtain odd vertices from edges
		VertexHandle EvenVertices[3];
		EvenVertices[0] = OddVertices[OldEdges[0]];
		EvenVertices[1] = OddVertices[OldEdges[1]];
		EvenVertices[2] = OddVertices[OldEdges[2]];

		// Obtain even vertices
		VertexHandle OddVertices[3];
		FaceVertexIter fv_it = fv_begin(f_it.handle());
		OddVertices[0] = fv_it.handle();
		OddVertices[1] = (++fv_it).handle();
		OddVertices[2] = (++fv_it).handle();

		// Store odd and even vertices
		std::vector<VertexHandle> VertexList;

		VertexList.push_back(OddVertices[0]);
		VertexList.push_back(OddVertices[1]);
		VertexList.push_back(OddVertices[2]);
		VertexList.push_back(EvenVertices[0]);
		VertexList.push_back(EvenVertices[1]);
		VertexList.push_back(EvenVertices[2]);

		VertexCollections.push_back(VertexList);
	}

	// Remove all old edges from the mesh
	for (EdgeIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		this->delete_edge(e_it.handle(), false);
	}

	// Permanently delete old edge data
	garbage_collection();

	// Add new faces to mesh
	for(std::vector<std::vector<VertexHandle>>::iterator i = VertexCollections.begin(); i != VertexCollections.end(); ++i)
	{
		std::vector<VertexHandle> face = *i;
		/*
		*			  face[2]
		*				/\
		*			   /  \
		*	  face[3] o    o face[5] 
		*            /      \
		*           /____o___\
		*	face[0]   face[4]  face[1]
		*/
		add_face(face[3], face[0], face[4]);
		add_face(face[2], face[3], face[5]);
		add_face(face[5], face[4], face[1]);
		add_face(face[3], face[4], face[5]);
	}

	// Apply even vertex mask
	for (std::vector<VertexHandle>::iterator v_it = EvenVertices.begin(); v_it != EvenVertices.end(); ++v_it)
	{
		// Vertex is on boundary: apply special mask
		if (is_boundary(*v_it))
		{
			Point NeighborPointSum = point(*v_it) * 3.0 / 4.0;
			TexCoord2D NeighborTexCoordSum;
			if (has_vertex_texcoords2D())
				NeighborTexCoordSum = texcoord2D(*v_it) * 3.0 / 4.0;

			for (VertexVertexIter vv_it = vv_begin(*v_it); vv_it != vv_end(*v_it); ++vv_it)
			{
				if (is_boundary(vv_it.handle()))
				{
					NeighborPointSum += point(vv_it.handle()) / 8.0;
					if (has_vertex_texcoords2D())
						NeighborTexCoordSum += texcoord2D(vv_it.handle()) / 8.0;
				}
			}

			set_point(*v_it, NeighborPointSum);
			if (has_vertex_texcoords2D())
				set_texcoord2D(*v_it, NeighborTexCoordSum);
		}
		else
		{
			// Vertex is not on boundary: apply normal mask
			int NeighborCount = 0;
			Point NeighborPointSum = Point(0.0f, 0.0f, 0.0f);
			TexCoord2D NeighborTexCoordSum;
			if (has_vertex_texcoords2D())
				NeighborTexCoordSum = TexCoord2D(0,0);

			for (VertexVertexIter vv_it = vv_begin(*v_it); vv_it != vv_end(*v_it); ++vv_it)
			{
				NeighborPointSum += point(vv_it.handle());
				if(has_vertex_texcoords2D())
					NeighborTexCoordSum += texcoord2D(vv_it.handle());
				NeighborCount++;
			}

			float B;

			if(NeighborCount > 3)
				B = 3.0f / (8.0f * NeighborCount);
			else
				B = 3.0f / 16.0f;

			set_point(*v_it, NeighborPointSum * B + (1 - NeighborCount * B) * point(*v_it));
			if(has_vertex_texcoords2D())
				set_texcoord2D(*v_it, NeighborTexCoordSum * B + (1 - NeighborCount * B) * texcoord2D(*v_it));
		}
	}

	// Regenerate normals
	GenerateNormals();

	return this;
}

PolyMesh *PolyMesh::Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	float MV[16];
	util::CopyMatrix16fv(ModelView, MV);

	if (Animated)
	{
		if (Animation->RDuration() > 0) {
			Quaternion QT = Animation->R(
				(AnimationRepeat) ? (Time - AnimationOffset - AnimationDelay) % Animation->RDuration()
				: Time - AnimationOffset - AnimationDelay);
			Rotate(360.0f*QT.w()/(2*((float)M_PI)), QT.x(), QT.y(), QT.z());
		}
		if (Animation->TDuration() > 0) {
			CatmullRom CR = Animation->T(
				(AnimationRepeat) ? (Time - AnimationOffset - AnimationDelay) % Animation->TDuration()
				: Time - AnimationOffset - AnimationDelay);
			Translate(CR.Vec3f()[0],CR.Vec3f()[1],CR.Vec3f()[2]);
		}
	}

	glMultMatrixf(ModelView);

	glShadeModel (ShadeMode);

	if(Lighting)
	{
		glEnable(GL_LIGHTING);

		float Clear[] = { 0.0f, 0.0f, 0.0f, 1.0f };

		if(MaterialAmbient != NULL)
			glMaterialfv(MaterialFaceMode, GL_AMBIENT, MaterialAmbient);
		else
			glMaterialfv(MaterialFaceMode, GL_AMBIENT, Clear);

		if(MaterialDiffuse != NULL)
			glMaterialfv(MaterialFaceMode, GL_DIFFUSE, MaterialDiffuse);
		else
			glMaterialfv(MaterialFaceMode, GL_DIFFUSE, Clear);

		if(MaterialEmission != NULL)
			glMaterialfv(MaterialFaceMode, GL_EMISSION, MaterialEmission);
		else
			glMaterialfv(MaterialFaceMode, GL_EMISSION, Clear);

		if(MaterialShininess != NULL)
			glMaterialfv(MaterialFaceMode, GL_SHININESS, MaterialShininess);
		else
			glMaterialfv(MaterialFaceMode, GL_SHININESS, Clear);

		if(MaterialSpecular != NULL)
			glMaterialfv(MaterialFaceMode, GL_SPECULAR, MaterialSpecular);
		else
			glMaterialfv(MaterialFaceMode, GL_SPECULAR, Clear);
	}
	else
	{
		if(Color != NULL)
			glColor4fv(Color);
		else
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if(has_vertex_texcoords2D() && Texture != NULL)
	{
		if(!glIsTexture(TextureID))
		{
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, TextureID);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glEnable(GL_TEXTURE_2D);
	}


	for (PolyMesh::FaceIter f_it=this->faces_begin(); f_it!=this->faces_end(); ++f_it)
	{
		glBegin(DrawMode);

		if (ShadeMode == GL_FLAT && this->has_face_normals())
			glNormal3f(this->normal(f_it.handle())[0], this->normal(f_it.handle())[1], this->normal(f_it.handle())[2]);

		for (PolyMesh::FaceVertexIter fv_it = this->fv_begin(f_it.handle()); fv_it != this->fv_end(f_it.handle()); ++fv_it)
		{
			if (ShadeMode == GL_SMOOTH && this->has_vertex_normals())
				glNormal3f(this->normal(fv_it.handle())[0], this->normal(fv_it.handle())[1],this->normal(fv_it.handle())[2]);

			if (this->has_vertex_texcoords2D())
				glTexCoord2f(this->texcoord2D(fv_it.handle())[0],this->texcoord2D(fv_it.handle())[1]);

			glVertex3f(this->point(fv_it.handle())[0], this->point(fv_it.handle())[1], this->point(fv_it.handle())[2]);
		}

		glEnd();
	}

	if(Lighting)
	{
		glDisable(GL_LIGHTING);
	}

	if (has_vertex_texcoords2D() && Texture != NULL)
	{
		glDisable(GL_TEXTURE_2D);
	}

	util::CopyMatrix16fv(MV, ModelView);

	glPopMatrix();

	return this;
}

PolyMesh *PolyMesh::LoadObj(std::string FilePath)
{
	std::ifstream file(FilePath);

	if (file.good())
	{
		std::vector<PolyMesh::VertexHandle> vHandle = std::vector<PolyMesh::VertexHandle>();
		vHandle.push_back(PolyMesh::VertexHandle());
		std::vector<OpenMesh::Vec3f> nHandle = std::vector<OpenMesh::Vec3f>();
		nHandle.push_back(OpenMesh::Vec3f());
		std::vector<OpenMesh::Vec2f> tHandle = std::vector<OpenMesh::Vec2f>();
		tHandle.push_back(OpenMesh::Vec2f());

		std::stringstream buffer;

		buffer << file.rdbuf();

		std::string token;
		buffer >> token;

		while (buffer.good())
		{
			if (token.compare("#") == 0)
			{
				buffer.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else if (token.compare("v") == 0)
			{
				float x, y, z;

				buffer >> x >> y >> z;

				max_x = std::max(max_x, x);
				min_x = std::min(min_x, x);

				max_y = std::max(max_y, y);
				min_y = std::min(min_y, y);

				max_z = std::max(max_z, z);
				min_z = std::min(min_z, z);

				vHandle.push_back(this->add_vertex(Point(x, y, z)));
			}
			else if (token.compare("vn") == 0)
			{
				float x, y, z;

				buffer >> x >> y >> z;

				nHandle.push_back(OpenMesh::Vec3f(x, y, z));
			}
			else if (token.compare("vt") == 0)
			{
				float x, y;

				buffer >> x >> y;

				tHandle.push_back(OpenMesh::Vec2f(x, y));
			}
			else if (token.compare("f") == 0)
			{
				int vertices[3];
				int normals[3];
				int textures[3];
				textures[0] = textures[1] = textures[2] = 0;
				normals[0] = normals[1] = normals[2] = 0;
				std::string vertexData;
				for (int i = 0; i < 3; i++)
				{
					char delimiter;
					buffer >> vertexData;
					std::stringstream stream (vertexData);
					stream >> vertices[i] >> delimiter >> textures[i] >> delimiter >> normals[i];
				}
				this->add_face(vHandle[vertices[0]], vHandle[vertices[1]], vHandle[vertices[2]]);
				if(textures[0] != 0 && textures[1] != 0 && textures[2] != 0)
				{
					this->request_vertex_texcoords2D();
					this->set_texcoord2D(vHandle[vertices[0]], tHandle[textures[0]]);
					this->set_texcoord2D(vHandle[vertices[1]], tHandle[textures[1]]);
					this->set_texcoord2D(vHandle[vertices[2]], tHandle[textures[2]]);

					this->request_vertex_normals();
					this->set_normal(vHandle[vertices[0]], nHandle[normals[0]]);
					this->set_normal(vHandle[vertices[1]], nHandle[normals[1]]);
					this->set_normal(vHandle[vertices[2]], nHandle[normals[2]]);
				}
			}
			else
			{
				buffer.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			buffer>>token;
		}
	}

	file.close();

	return this;
}

PolyMesh *PolyMesh::Animate(AnimationRoutine *animation, uint64_t delay, bool repeat)
{
	Animation = animation;
	AnimationDelay = delay;
	AnimationRepeat = repeat;
	AnimationOffset = Time;
	Animated = true;
	return this;
}

void PolyMesh::Delete()
{
	clear();
}