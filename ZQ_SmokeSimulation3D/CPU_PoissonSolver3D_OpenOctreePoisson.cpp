#include "CPU_PoissonSolver3D_OpenOctreePoisson.h"
#include "ZQ_PoissonSolver3D.h"

using namespace ZQ::ZQ_PoissonSolver3D;

void Adjust_MAC_u_OpenOctreePoisson_Kernel(float* mac_u, const float* p_level0, const float* p_level1, const float* p_level2, const float* p_level3, 
						const bool* leaf0, const bool* leaf1, const bool* leaf2, const bool* leaf3, const int width, const int height, const int depth)
{

	for(int z = 0;z < depth;z++)
	{
		for(int y = 0;y < height;y++)
		{
			for(int x = 0;x <= width;x++)
			{
				float len1 = -1, len2 = -1;
				if(x == 0)
				{
					if(leaf0[z*height*width+y*width+x])
					{
						len1 = 4.0f;
						len2 = 0.5f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (p_level0[(z*height*width+y*width+x)] - 0)/(len1+len2);
					}
					else if(leaf1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2])
					{
						len1 = 4.0f;
						len2 = 1.0f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (p_level1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2] - 0)/(len1+len2);
					}
					else if(leaf2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4])
					{
						len1 = 4.0f;
						len2 = 2.0f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (p_level2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4] - 0)/(len1+len2);
					}
					else if(leaf3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8])
					{
						len1 = 4.0f;
						len2 = 4.0f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (p_level3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8] - 0)/(len1+len2);
					}
				}
				else if(x == width)
				{
					if(leaf0[z*height*width+y*width+x-1])
					{
						len1 = 4.0f;
						len2 = 0.5f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (0 - p_level0[z*height*width+y*width+x-1])/(len1+len2);
					}
					else if(leaf1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2-1])
					{
						len1 = 4.0f;
						len2 = 1.0f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (0 - p_level1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2-1])/(len1+len2);
					}
					else if(leaf2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4-1])
					{
						len1 = 4.0f;
						len2 = 2.0f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (0 - p_level2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4-1])/(len1+len2);
					}
					else if(leaf3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8-1])
					{
						len1 = 4.0f;
						len2 = 4.0f;
						mac_u[z*height*(width+1)+y*(width+1)+x] -= (0 - p_level3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8-1])/(len1+len2);
					}
				}
				else
				{
					float p1,p2;
					if(leaf0[z*height*width+y*width+x-1])
					{
						len1 = 0.5f;
						p1 = p_level0[z*height*width+y*width+x-1];
					}
					else if(x%2 == 0 && leaf1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2-1])
					{
						len1 = 1.0f;
						p1 = p_level1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2-1];
					}
					else if(x%4 == 0 && leaf2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4-1])
					{
						len1 = 2.0f;
						p1 = p_level2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4-1];
					}
					else if(x%8 == 0 && leaf3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8-1])
					{
						len1 = 4.0f;
						p1 = p_level3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8-1];
					}

					if(len1 < 0)
						return ;

					if(leaf0[z*height*width+y*width+x])
					{
						len2 = 0.5f;
						p2 = p_level0[z*height*width+y*width+x];
					}
					else if(x%2 == 0 && leaf1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2])
					{
						len2 = 1.0f;
						p2 = p_level1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2];
					}
					else if(x%4 == 0 && leaf2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4])
					{
						len2 = 2.0f;
						p2 = p_level2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4];
					}
					else if(x%8 == 0 && leaf3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8])
					{
						len2 = 4.0f;
						p2 = p_level3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8];
					}

					if(len2 < 0)
						return;
					mac_u[z*height*(width+1)+y*(width+1)+x] -= (p2-p1)/(len1+len2);
				}
			}
		}
	}
}

void Adjust_MAC_v_OpenOctreePoisson_Kernel(float* mac_v, const float* p_level0, const float* p_level1, const float* p_level2, const float* p_level3, 
							const bool* leaf0,const bool* leaf1, const bool* leaf2, const bool* leaf3, const int width, const int height, const int depth)
{
	for(int z = 0;z < depth;z++)
	{
		for(int y = 0;y <= height;y++)
		{
			for(int x = 0;x < width;x++)
			{
				float len1 = -1, len2 = -1;
				if(y == 0)
				{
					if(leaf0[z*height*width+y*width+x])
					{
						len1 = 4.0f;
						len2 = 0.5f;
						mac_v[z*(height+1)*width+y*width+x] -= (p_level0[z*height*width+y*width+x] - 0)/(len1+len2);
					}
					else if(leaf1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2])
					{
						len1 = 4.0f;
						len2 = 1.0f;
						mac_v[z*(height+1)*width+y*width+x] -= (p_level1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2] - 0)/(len1+len2);
					}
					else if(leaf2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4])
					{
						len1 = 4.0f;
						len2 = 2.0f;
						mac_v[z*(height+1)*width+y*width+x] -= (p_level2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4] - 0)/(len1+len2);
					}
					else if(leaf3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8])
					{
						len1 = 4.0f;
						len2 = 4.0f;
						mac_v[z*(height+1)*width+y*width+x] -= (p_level3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8] - 0)/(len1+len2);
					}
				}
				else if(y == height)
				{
					if(leaf0[z*height*width+(y-1)*width+x])
					{
						len1 = 4.0f;
						len2 = 0.5f;
						mac_v[z*(height+1)*width+y*width+x] -= (0 - p_level0[z*height*width+(y-1)*width+x])/(len1+len2);
					}
					else if(leaf1[z/2*(height/2)*(width/2)+(y/2-1)*(width/2)+x/2])
					{
						len1 = 4.0f;
						len2 = 1.0f;
						mac_v[z*(height+1)*width+y*width+x] -= (0 - p_level1[z/2*(height/2)*(width/2)+(y/2-1)*(width/2)+x/2])/(len1+len2);
					}
					else if(leaf2[z/4*(height/4)*(width/4)+(y/4-1)*(width/4)+x/4])
					{
						len1 = 4.0f;
						len2 = 2.0f;
						mac_v[z*(height+1)*width+y*width+x] -= (0 - p_level2[z/4*(height/4)*(width/4)+(y/4-1)*(width/4)+x/4])/(len1+len2);
					}
					else if(leaf3[z/8*(height/8)*(width/8)+(y/8-1)*(width/8)+x/8])
					{
						len1 = 4.0f;
						len2 = 4.0f;
						mac_v[z*(height+1)*width+y*width+x] -= (0 - p_level3[z/8*(height/8)*(width/8)+(y/8-1)*(width/8)+x/8])/(len1+len2);
					}
				}
				else
				{
					float p1,p2;
					if(leaf0[z*height*width+(y-1)*width+x])
					{
						len1 = 0.5f;
						p1 = p_level0[z*height*width+(y-1)*width+x];
					}
					else if(y%2 == 0 && leaf1[z/2*(height/2)*(width/2)+(y/2-1)*(width/2)+x/2])
					{
						len1 = 1.0f;
						p1 = p_level1[z/2*(height/2)*(width/2)+(y/2-1)*(width/2)+x/2];
					}
					else if(y%4 == 0 && leaf2[z/4*(height/4)*(width/4)+(y/4-1)*(width/4)+x/4])
					{
						len1 = 2.0f;
						p1 = p_level2[z/4*(height/4)*(width/4)+(y/4-1)*(width/4)+x/4];
					}
					else if(y%8 == 0 && leaf3[z/8*(height/8)*(width/8)+(y/8-1)*(width/8)+x/8])
					{
						len1 = 4.0f;
						p1 = p_level3[z/8*(height/8)*(width/8)+(y/8-1)*(width/8)+x/8];
					}

					if(len1 < 0)
						return;

					if(leaf0[z*height*width+y*width+x])
					{
						len2 = 0.5f;
						p2 = p_level0[z*height*width+y*width+x];
					}
					else if(y%2 == 0 && leaf1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2])
					{
						len2 = 1.0f;
						p2 = p_level1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2];
					}
					else if(y%4 == 0 && leaf2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4])
					{
						len2 = 2.0f;
						p2 = p_level2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4];
					}
					else if(y%8 == 0 && leaf3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8])
					{
						len2 = 4.0f;
						p2 = p_level3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8];
					}

					if(len2 < 0)
						return ;	

					mac_v[z*(height+1)*width+y*width+x] -= (p2-p1)/(len1+len2);
				}
			}
		}
	}
}

void Adjust_MAC_w_OpenOctreePoisson_Kernel(float* mac_w, const float* p_level0, const float* p_level1, const float* p_level2, const float* p_level3, 
							const bool* leaf0,const bool* leaf1, const bool* leaf2, const bool* leaf3, const int width, const int height, const int depth)
{
	

	for(int y = 0;y < height;y++)
	{
		for(int x = 0;x < width;x++)
		{
			float len1 = -1, len2 = -1;
			//z == 0
			if(leaf0[y*width+x])
			{
				len1 = 4.0f;
				len2 = 0.5f;
				mac_w[y*width+x] -= (p_level0[y*width+x] - 0)/(len1+len2);
			}
			else if(leaf1[y/2*(width/2)+x/2])
			{
				len1 = 4.0f;
				len2 = 1.0f;
				mac_w[y*width+x] -= (p_level1[y/2*(width/2)+x/2] - 0)/(len1+len2);
			}
			else if(leaf2[y/4*(width/4)+x/4])
			{
				len1 = 4.0f;
				len2 = 2.0f;
				mac_w[y*width+x] -= (p_level2[y/4*(width/4)+x/4] - 0)/(len1+len2);
			}
			else if(leaf3[y/8*(width/8)+x/8])
			{
				len1 = 4.0f;
				len2 = 4.0f;
				mac_w[y*width+x] -= (p_level3[y/8*(width/8)+x/8] - 0)/(len1+len2);
			}

			//z == depth
			len1 = -1; 
			len2 = -1;
			if(leaf0[(depth-1)*height*width+y*width+x])
			{
				len1 = 4.0f;
				len2 = 0.5f;
				mac_w[depth*height*width+y*width+x] -= (0 - p_level0[(depth-1)*height*width+y*width+x])/(len1+len2);
			}
			else if(leaf1[(depth/2-1)*(height/2)*(width/2)+y/2*(width/2)+x/2])
			{
				len1 = 4.0f;
				len2 = 1.0f;
				mac_w[depth*height*width+y*width+x] -= (0 - p_level1[(depth/2-1)*(height/2)*(width/2)+y/2*(width/2)+x/2])/(len1+len2);
			}
			else if(leaf2[(depth/4-1)*(height/4)*(width/4)+y/4*(width/4)+x/4])
			{
				len1 = 4.0f;
				len2 = 2.0f;
				mac_w[depth*height*width+y*width+x] -= (0 - p_level2[(depth/4-1)*(height/4)*(width/4)+y/4*(width/4)+x/4])/(len1+len2);
			}
			else if(leaf3[(depth/8-1)*(height/8)*(width/8)+y/8*(width/8)+x/8])
			{
				len1 = 4.0f;
				len2 = 4.0f;
				mac_w[depth*height*width+y*width+x] -= (0 - p_level3[(depth/8-1)*(height/8)*(width/8)+y/8*(width/8)+x/8])/(len1+len2);
			}

			for(int z = 1;z < depth;z++)
			{
				len1 = -1;
				len2 = -1;

				float p1,p2;
				if(leaf0[(z-1)*height*width+y*width+x])
				{
					len1 = 0.5f;
					p1 = p_level0[(z-1)*height*width+y*width+x];
				}
				else if(z%2 == 0 && leaf1[(z/2-1)*(height/2)*(width/2)+y/2*(width/2)+x/2])
				{
					len1 = 1.0f;
					p1 = p_level1[(z/2-1)*(height/2)*(width/2)+y/2*(width/2)+x/2];
				}
				else if(z%4 == 0 && leaf2[(z/4-1)*(height/4)*(width/4)+y/4*(width/4)+x/4])
				{
					len1 = 2.0f;
					p1 = p_level2[(z/4-1)*(height/4)*(width/4)+y/4*(width/4)+x/4];
				}
				else if(z%8 == 0 && leaf3[(z/8-1)*(height/8)*(width/8)+y/8*(width/8)+x/8])
				{
					len1 = 4.0f;
					p1 = p_level3[(z/8-1)*(height/8)*(width/8)+y/8*(width/8)+x/8];
				}

				if(len1 < 0)
					continue;

				if(leaf0[z*height*width+y*width+x])
				{
					len2 = 0.5f;
					p2 = p_level0[z*height*width+y*width+x];
				}
				else if(z%2 == 0 && leaf1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2])
				{
					len2 = 1.0f;
					p2 = p_level1[z/2*(height/2)*(width/2)+y/2*(width/2)+x/2];
				}
				else if(z%4 == 0 && leaf2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4])
				{
					len2 = 2.0f;
					p2 = p_level2[z/4*(height/4)*(width/4)+y/4*(width/4)+x/4];
				}
				else if(z%8 == 0 && leaf3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8])
				{
					len2 = 4.0f;
					p2 = p_level3[z/8*(height/8)*(width/8)+y/8*(width/8)+x/8];
				}

				if(len2 < 0)
					continue ;	

				mac_w[z*height*width+y*width+x] -= (p2-p1)/(len1+len2);
			}
		}
	}	
}

void Calculate_Divergence_Octree_from_previous_level_Kernel(float* cur_level_divergence, const float* pre_level_divergence, 
											const int cur_level_width, const int cur_level_height, const int cur_level_depth)
{

	int width = 2*cur_level_width;
	int height = 2*cur_level_height;

	for(int z = 0;z < cur_level_depth;z++)
	{
		for(int y = 0;y < cur_level_height;y++)
		{
			for(int x = 0;x < cur_level_width;x++)
			{
				cur_level_divergence[z*cur_level_height*cur_level_width+y*cur_level_width+x] = 
					pre_level_divergence[(z*2  )*height*width+(y*2  )*width+x*2  ]
				+ pre_level_divergence[(z*2  )*height*width+(y*2  )*width+x*2+1]
				+ pre_level_divergence[(z*2  )*height*width+(y*2+1)*width+x*2  ]
				+ pre_level_divergence[(z*2  )*height*width+(y*2+1)*width+x*2+1]
				+ pre_level_divergence[(z*2+1)*height*width+(y*2  )*width+x*2  ]
				+ pre_level_divergence[(z*2+1)*height*width+(y*2  )*width+x*2+1]
				+ pre_level_divergence[(z*2+1)*height*width+(y*2+1)*width+x*2  ]
				+ pre_level_divergence[(z*2+1)*height*width+(y*2+1)*width+x*2+1];
			}
		}
	}
}

void SolvePressure_OpenOctreePoisson_level0_RedBlack_Kernel(float* p_level0, const float* p_level1, const float* p_level2, const float* p_level3, const float* divergence0, 
															const int width, const int height, const int depth,
															const int level0_num, const int* level0_index, const int* level0_neighborinfo)
{
	const int index_channels = 5;
	const int neighborinfo_channels = 4;

	for(int cur_idx = 0;cur_idx < level0_num; cur_idx++)
	{
		int x = level0_index[cur_idx*index_channels+0];
		int y = level0_index[cur_idx*index_channels+1];
		int z = level0_index[cur_idx*index_channels+2];
		int num = level0_index[cur_idx*index_channels+3];
		int offset = level0_index[cur_idx*index_channels+4];

		float coeff = 0,sigma = 0;
		const float weight[4] = {1.0f/(0.5f+0.5f),1.0f/(0.5f+1.0f),1.0f/(0.5f+2.0f),1.0f/(0.5f+4.0f)};
		int imWidth[4] = {width,width/2,width/4,width/8};
		int imHeight[4] = {height,height/2,height/4,height/8};
		int imDepth[4] = {depth,depth/2,depth/4,depth/8};
		const float* pressure[4] = {p_level0,p_level1,p_level2,p_level3};
		for(int nn = 0;nn < num;nn++)
		{
			int neighbor_offset = (offset+nn)*neighborinfo_channels;
			int neighbor_lvl = level0_neighborinfo[neighbor_offset+0];
			int neighbor_x = level0_neighborinfo[neighbor_offset+1]; 
			int neighbor_y = level0_neighborinfo[neighbor_offset+2];
			int neighbor_z = level0_neighborinfo[neighbor_offset+3];

			float cur_weight = weight[neighbor_lvl];
			coeff += cur_weight;
			if(neighbor_x >= 0 && neighbor_y >= 0 && neighbor_x < imWidth[neighbor_lvl] && neighbor_y < imHeight[neighbor_lvl]
			&& neighbor_z >= 0 && neighbor_z < imDepth[neighbor_lvl])
				sigma += cur_weight * pressure[neighbor_lvl][neighbor_z*imHeight[neighbor_lvl]*imWidth[neighbor_lvl]+neighbor_y*imWidth[neighbor_lvl]+neighbor_x];
		}

		sigma -= divergence0[z*height*width+y*width+x];
		p_level0[z*height*width+y*width+x] = sigma/coeff;	
	}	
}

void SolvePressure_OpenOctreePoisson_level1_RedBlack_Kernel(const float* p_level0, float* p_level1, const float* p_level2, const float* p_level3, const float* divergence1, 
															const int width, const int height, const int depth,
															const int level1_num, const int* level1_index, const int* level1_neighborinfo)
{
	const int index_channels = 5;
	const int neighborinfo_channels = 4;

	for(int cur_idx = 0; cur_idx < level1_num;cur_idx++)
	{
		int x = level1_index[cur_idx*index_channels+0];
		int y = level1_index[cur_idx*index_channels+1];
		int z = level1_index[cur_idx*index_channels+2];
		int num = level1_index[cur_idx*index_channels+3];
		int offset = level1_index[cur_idx*index_channels+4];

		float coeff = 0,sigma = 0;
		const float weight[4] = {1.0f/(1.0f+0.5f),4.0f/(1.0f+1.0f),4.0f/(1.0f+2.0f),4.0f/(1.0f+4.0f)};
		int imWidth[4] = {width,width/2,width/4,width/8};
		int imHeight[4] = {height,height/2,height/4,height/8};
		int imDepth[4] = {depth, depth/2, depth/4, depth/8};
		const float* pressure[4] = {p_level0,p_level1,p_level2,p_level3};
		for(int nn = 0;nn < num;nn++)
		{
			int neighbor_offset = (offset+nn)*neighborinfo_channels;
			int neighbor_lvl = level1_neighborinfo[neighbor_offset+0];
			int neighbor_x = level1_neighborinfo[neighbor_offset+1]; 
			int neighbor_y = level1_neighborinfo[neighbor_offset+2];
			int neighbor_z = level1_neighborinfo[neighbor_offset+3];

			float cur_weight = weight[neighbor_lvl];
			coeff += cur_weight;
			if(neighbor_x >= 0 && neighbor_y >= 0 && neighbor_x < imWidth[neighbor_lvl] && neighbor_y < imHeight[neighbor_lvl]
			&& neighbor_z >= 0 && neighbor_z < imDepth[neighbor_lvl])
				sigma += cur_weight * pressure[neighbor_lvl][neighbor_z*imHeight[neighbor_lvl]*imWidth[neighbor_lvl]+neighbor_y*imWidth[neighbor_lvl]+neighbor_x];
		}

		sigma -= divergence1[z*(height/2)*(width/2)+y*(width/2)+x];
		p_level1[z*(height/2)*(width/2)+y*(width/2)+x] = sigma/coeff;	
	}	
}

void SolvePressure_OpenOctreePoisson_level2_RedBlack_Kernel(const float* p_level0, const float* p_level1, float* p_level2, const float* p_level3, const float* divergence2, 
															const int width, const int height, const int depth,
															const int level2_num, const int* level2_index, const int* level2_neighborinfo)
{
	const int index_channels = 5;
	const int neighborinfo_channels = 4;

	for(int cur_idx = 0;cur_idx < level2_num;cur_idx++)
	{
		int x = level2_index[cur_idx*index_channels+0];
		int y = level2_index[cur_idx*index_channels+1];
		int z = level2_index[cur_idx*index_channels+2];
		int num = level2_index[cur_idx*index_channels+3];
		int offset = level2_index[cur_idx*index_channels+4];

		float coeff = 0,sigma = 0;
		const float weight[4] = {1.0f/(2.0f+0.5f),4.0f/(2.0f+1.0f),16.0f/(2.0f+2.0f),16.0f/(2.0f+4.0f)};
		int imWidth[4] = {width,width/2,width/4,width/8};
		int imHeight[4] = {height,height/2,height/4,height/8};
		int imDepth[4] = {depth,depth/2,depth/4,depth/8};
		const float* pressure[4] = {p_level0,p_level1,p_level2,p_level3};
		for(int nn = 0;nn < num;nn++)
		{
			int neighbor_offset = (offset+nn)*neighborinfo_channels;
			int neighbor_lvl = level2_neighborinfo[neighbor_offset+0];
			int neighbor_x = level2_neighborinfo[neighbor_offset+1]; 
			int neighbor_y = level2_neighborinfo[neighbor_offset+2];
			int neighbor_z = level2_neighborinfo[neighbor_offset+3];

			float cur_weight = weight[neighbor_lvl];
			coeff += cur_weight;
			if(neighbor_x >= 0 && neighbor_y >= 0 && neighbor_x < imWidth[neighbor_lvl] && neighbor_y < imHeight[neighbor_lvl]
			&& neighbor_z >= 0 && neighbor_z < imDepth[neighbor_lvl])
				sigma += cur_weight * pressure[neighbor_lvl][neighbor_z*imHeight[neighbor_lvl]*imWidth[neighbor_lvl]+neighbor_y*imWidth[neighbor_lvl]+neighbor_x];
		}

		sigma -= divergence2[z*(height/4)*(width/4)+y*(width/4)+x];
		p_level2[z*(height/4)*(width/4)+y*(width/4)+x] = sigma/coeff;
	}		
}

void SolvePressure_OpenOctreePoisson_level3_RedBlack_Kernel(const float* p_level0, const float* p_level1, const float* p_level2, float* p_level3, const float* divergence3, 
															const int width, const int height, const int depth,
															const int level3_num, const int* level3_index, const int* level3_neighborinfo)
{
	const int index_channels = 5;
	const int neighborinfo_channels = 4;

	for(int cur_idx = 0;cur_idx < level3_num;cur_idx++)
	{
		int x = level3_index[cur_idx*index_channels+0];
		int y = level3_index[cur_idx*index_channels+1];
		int z = level3_index[cur_idx*index_channels+2];
		int num = level3_index[cur_idx*index_channels+3];
		int offset = level3_index[cur_idx*index_channels+4];

		float coeff = 0,sigma = 0;
		const float weight[4] = {1.0f/(4.0f+0.5f),4.0f/(4.0f+1.0f),16.0f/(4.0f+2.0f),64.0f/(4.0f+4.0f)};
		int imWidth[4] = {width,width/2,width/4,width/8};
		int imHeight[4] = {height,height/2,height/4,height/8};
		int imDepth[4] = {depth,depth/2,depth/4,depth/8};
		const float* pressure[4] = {p_level0,p_level1,p_level2,p_level3};
		for(int nn = 0;nn < num;nn++)
		{
			int neighbor_offset = (offset+nn)*neighborinfo_channels;
			int neighbor_lvl = level3_neighborinfo[neighbor_offset+0];
			int neighbor_x = level3_neighborinfo[neighbor_offset+1]; 
			int neighbor_y = level3_neighborinfo[neighbor_offset+2];
			int neighbor_z = level3_neighborinfo[neighbor_offset+3];

			float cur_weight = weight[neighbor_lvl];
			coeff += cur_weight;
			if(neighbor_x >= 0 && neighbor_y >= 0 && neighbor_x < imWidth[neighbor_lvl] && neighbor_y < imHeight[neighbor_lvl]
			&& neighbor_z >= 0 && neighbor_z < imDepth[neighbor_lvl])
				sigma += cur_weight * pressure[neighbor_lvl][neighbor_z*imHeight[neighbor_lvl]*imWidth[neighbor_lvl]+neighbor_y*imWidth[neighbor_lvl]+neighbor_x];
		}

		sigma -= divergence3[z*(height/8)*(width/8)+y*(width/8)+x];
		p_level3[z*(height/8)*(width/8)+y*(width/8)+x] = sigma/coeff;
	}
			
}

void CPU_SolveOpenOctreePoissonRedBlack3_MAC(float* mac_u, float* mac_v, float* mac_w, const bool* leaf0, const bool* leaf1, const bool* leaf2, const bool* leaf3, 
											 const int width, const int height, const int depth, const int maxIter,
											 const int level0_num_red, const int* level0_index_red, const int level0_info_len_red, const int* level0_neighborinfo_red,
											 const int level0_num_black, const int* level0_index_black, const int level0_info_len_black, const int* level0_neighborinfo_black,
											 const int level1_num_red, const int* level1_index_red, const int level1_info_len_red, const int* level1_neighborinfo_red, 
											 const int level1_num_black, const int* level1_index_black, const int level1_info_len_black, const int* level1_neighborinfo_black,
											 const int level2_num_red, const int* level2_index_red, const int level2_info_len_red, const int* level2_neighborinfo_red,
											 const int level2_num_black, const int* level2_index_black, const int level2_info_len_black, const int* level2_neighborinfo_black,
											 const int level3_num_red, const int* level3_index_red, const int level3_info_len_red, const int* level3_neighborinfo_red,
											 const int level3_num_black, const int* level3_index_black, const int level3_info_len_black, const int* level3_neighborinfo_black)
{
	int levelWidth1 = width/2;
	int levelWidth2 = width/4;
	int levelWidth3 = width/8;
	int levelHeight1 = height/2;
	int levelHeight2 = height/4;
	int levelHeight3 = height/8;
	int levelDepth1 = depth/2;
	int levelDepth2 = depth/4;
	int levelDepth3 = depth/8;

	float* divergence0 = new float[width*height*depth];
	float* divergence1 = new float[levelWidth1*levelHeight1*levelDepth1];
	float* divergence2 = new float[levelWidth2*levelHeight2*levelDepth2];
	float* divergence3 = new float[levelWidth3*levelHeight3*levelDepth3];
	
	float* p0 = new float[width*height*depth];
	float* p1 = new float[levelWidth1*levelHeight1*levelDepth1];
	float* p2 = new float[levelWidth2*levelHeight2*levelDepth2];
	float* p3 = new float[levelWidth3*levelHeight3*levelDepth3];
	

	for(int k = 0;k < depth;k++)
	{
		for(int j = 0;j < height;j++)
		{
			for(int i = 0;i < width;i++)
			{
				divergence0[k*height*width+j*width+i] = 
					mac_u[k*height*(width+1)+j*(width+1)+i+1] - mac_u[k*height*(width+1)+j*(width+1)+i]
				  + mac_v[k*(height+1)*width+(j+1)*width+i] - mac_v[k*(height+1)*width+j*width+i]
				  + mac_w[(k+1)*height*width+j*width+i] - mac_w[k*height*width+j*width+i];
			}
		}
	}

	Calculate_Divergence_Octree_from_previous_level_Kernel(divergence1,divergence0,levelWidth1,levelHeight1,levelDepth1);
	Calculate_Divergence_Octree_from_previous_level_Kernel(divergence2,divergence1,levelWidth2,levelHeight2,levelDepth2);
	Calculate_Divergence_Octree_from_previous_level_Kernel(divergence3,divergence2,levelWidth3,levelHeight3,levelDepth3);


	for(int it = 0;it < maxIter;it++)
	{
		if(level0_num_red > 0)
			SolvePressure_OpenOctreePoisson_level0_RedBlack_Kernel(
			p0, p1, p2, p3, divergence0, width, height, depth, level0_num_red,level0_index_red,level0_neighborinfo_red);
		if(level0_num_black > 0)
			SolvePressure_OpenOctreePoisson_level0_RedBlack_Kernel(
			p0, p1, p2, p3, divergence0, width, height, depth, level0_num_black,level0_index_black,level0_neighborinfo_black);

		if(level1_num_red > 0)
			SolvePressure_OpenOctreePoisson_level1_RedBlack_Kernel(
			p0, p1, p2, p3, divergence1, width, height, depth, level1_num_red,level1_index_red,level1_neighborinfo_red);

		if(level1_num_black > 0)
			SolvePressure_OpenOctreePoisson_level1_RedBlack_Kernel(
			p0, p1, p2, p3, divergence1, width, height, depth, level1_num_black,level1_index_black,level1_neighborinfo_black);

		if(level2_num_red > 0)
			SolvePressure_OpenOctreePoisson_level2_RedBlack_Kernel(
			p0, p1, p2, p3, divergence2, width, height, depth, level2_num_red,level2_index_red,level2_neighborinfo_red);

		if(level2_num_black > 0)
			SolvePressure_OpenOctreePoisson_level2_RedBlack_Kernel(
			p0, p1, p2, p3, divergence2, width, height, depth, level2_num_black,level2_index_black,level2_neighborinfo_black);	

		if(level3_num_red > 0)
			SolvePressure_OpenOctreePoisson_level3_RedBlack_Kernel(
			p0, p1, p2, p3, divergence3, width, height, depth, level3_num_red,level3_index_red,level3_neighborinfo_red);

		if(level3_num_black > 0)
			SolvePressure_OpenOctreePoisson_level3_RedBlack_Kernel(
			p0, p1, p2, p3, divergence3, width, height, depth, level3_num_black,level3_index_black,level3_neighborinfo_black);			

	}

	Adjust_MAC_u_OpenOctreePoisson_Kernel(mac_u, p0, p1, p2, p3, leaf0, leaf1, leaf2, leaf3, width, height, depth);
	Adjust_MAC_v_OpenOctreePoisson_Kernel(mac_v, p0, p1, p2, p3, leaf0, leaf1, leaf2, leaf3, width, height, depth);
	Adjust_MAC_w_OpenOctreePoisson_Kernel(mac_w, p0, p1, p2, p3, leaf0, leaf1, leaf2, leaf3, width, height, depth);

	delete []divergence0;
	delete []divergence1;
	delete []divergence2;
	delete []divergence3;
	delete []p0;
	delete []p1;
	delete []p2;
	delete []p3;

}
