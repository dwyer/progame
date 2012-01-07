#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <assimp.h>
#include <aiScene.h>
#include <aiPostProcess.h>

void node_print(struct aiNode *node) {
	 printf("Node Name: %s\n", node->mName.data);
}

void node_visit(struct aiNode *node, void (*func)(struct aiNode *)) {
	 int i;

	 assert(node);
	 func(node);

	 for (i = 0; i < node->mNumChildren; ++i)
		  node_visit(node->mChildren[i], func);
}

int main(int argc, char **argv) {
	 const struct aiScene *const_scene;
	 struct aiScene *scene;

	 const_scene = aiImportFile("../res/test/assimp_basic.dae",
								aiProcess_Triangulate);
	 scene = (struct aiScene *)const_scene;
	 assert(scene);

	 /* Traverse the node hierarchy and print the name of each node */

	 node_visit(scene->mRootNode,
				node_print);

	 return EXIT_SUCCESS;
}
