以下内容皆来自 arXiv:2004.03630 [cs]

## 运算符

### 基本运算符

#### 几何变换

$$
C^{\prime}=\mathscr{G}[\gamma](C)
$$

1. 根据坐标变换：$C^{\prime}(\gamma(x, y))=C(x, y)$
2. 根据值变换：$C^{\prime}(\gamma(C(x, y)))=C(x, y)$

#### 值变换

$$
C^{\prime}=\mathscr{V}[f](C)
\\
f: \mathbb{R}^{2} \times S^{3} \rightarrow S^{3}
$$

根据值和坐标变换：$C^{\prime}(x, y)=f(x, y, C(x, y))$

#### 遮罩

$$
C^{\prime}=\mathscr{M}[M](C)
$$

若值不符合要求，则抛弃之：

$C^{\prime}(x, y)=\left\{\begin{array}{ll}C(x, y), & \text { if } C(x, y) \in M \\ \emptyset & \text { otherwise }\end{array}\right.$

#### 混合

$$
C^{\prime}=\mathscr{B}[\odot]\left(C_{1}, C_{2}\right)\\
\odot: S^{3} \times S^{3} \rightarrow S^{3}
$$

根据两旧值产生新值：

 $C^{\prime}(x, y)= C_{1}(x, y) \odot C_{2}(x, y)$

#### 切分 (Dissect)

$$
\left\{C_{1}, C_{2}, \ldots, C_{n}\right\}=\mathscr{D}(C)
$$

每个非空像素点产生一张新 canvas：

$C_{i}\left(x^{\prime}, y^{\prime}\right)=\left\{\begin{array}{ll}C(x, y), & \text { if }\left(x^{\prime}, y^{\prime}\right)=(x, y) \\ \emptyset & \text { otherwise }\end{array}\right.$

### 派生运算符

#### 多重混合

$$
\begin{aligned}
C^{\prime} = & \mathscr{B}^{*}[\odot]\left(C_{1}, C_{2}, \ldots, C_{n}\right)\\
  = & \mathscr{B}[\odot]\left(C_{1}, \mathscr{B}[\odot]\left(C_{2}, \mathscr{B}[\odot]\left(C_{3}, \ldots\right)\right)\right)
\end{aligned}
$$

#### 映射

$$
\begin{aligned}
\left\{C_{1}, C_{2}, \ldots, C_{n}\right\}=&\mathscr{D}^{*}[\gamma](C)\\
=& \mathscr{G}[\gamma](\mathscr{D}(C))

\end{aligned}
$$

## 表达

### 选择查询

#### 选点 

![select_points](./assets/select_points.png)

$C_{i}(x, y)[0]=\left\{\begin{array}{ll}(i d, 1,0) & \text { if }(x, y)=\left(x_{i}, y_{i}\right) \\ \emptyset & \text { otherwise }\end{array}\right.$

$C_{i}(x, y)[1]=C_{i}(x, y)[2]=\emptyset$

$\begin{aligned} C_{Q}(x, y)[0] &=C_{Q}(x, y)[1]=\emptyset \\ C_{Q}(x, y)[2] &=\left\{\begin{array}{ll}(1,1,0) & \text { if }(x, y) \text { falls inside } Q \\ \emptyset & \text { otherwise }\end{array}\right.\end{aligned}$

$\mathbb{C}_{\text {result }} \leftarrow \mathscr{M}\left[M_{p}\right]\left(\mathscr{B}[\odot]\left(\mathbb{C}_{P}, C_{Q}\right)\right)$

$s_{1} \odot s_{2}=\left[\begin{array}{ccc}s_{1}[0][0] & s_{1}[0][1] & s_{1}[0][2] \\ - & \emptyset & - \\ s_{2}[2][0] & s_{2}[2][1] & s_{2}[2][2]\end{array}\right]$

$M_{p}=\left\{s \in S^{3} \mid s[0] \neq \emptyset\right.$ and $\left.s[2][0]=1\right\}$

#### 选多边形

![select_polygons](./assets/select_polygons.png)

$\begin{aligned} C_{i}(x, y)[0] &=C_{i}(x, y)[1]=\emptyset \\ C_{i}(x, y)[2] &=\left\{\begin{array}{ll}(i d, 1,0) & \text { if }(x, y) \text { falls inside } Y_{i} \\ \emptyset & \text { otherwise }\end{array}\right.\end{aligned}$

$\mathbb{C}_{\text {result }} \leftarrow \mathscr{M}\left[M_{y}\right]\left(\mathscr{B}[\oplus]\left(\mathbb{C}_{Y}, C_{Q}\right)\right)$

$s_{1} \oplus s_{2}=\left[\begin{array}{ccc}- & \emptyset & - \\ - & \emptyset & - \\ s_{1}[2][0] & s_{1}[2][1]+s_{2}[2][1] & s_{1}[2][2]\end{array}\right]$

$M_{y}=\left\{s \in S^{3} \mid s[2][1]=2\right\}$

### 连接 (Join) 查询

[TODO]

### 聚合 (Aggregate) 查询

SELECT COUNT (*) FROM $D_P$ WHERE Location INSIDE $Q$

 $C_{\text {count }} \leftarrow \mathscr{B}^{*}[+]\left(\mathscr{G}\left[\gamma_{c}\right]\left(\mathbb{C}_{\text {result }}\right)\right)$

 $\gamma_{c}: S^{3} \rightarrow \mathbb{R}^{2}: \gamma_{c}(s)=(s[2][0], 0), \forall s \in S^{3} ;$ 

$+: S^{3} \times S^{3} \rightarrow S^{3}$ : 
$$
s_{1}+s_{2}=\left[\begin{array}{ccc}
0 & s_{1}[0][1]+s_{2}[0][1] & 0 \\
- & \emptyset & - \\
s_{2}[2][0] & s_{2}[2][1] & s_{2}[2][2]
\end{array}\right]
$$
$\mathbb{C}_{\text {result }} \leftarrow \mathscr{M}\left[M_{p}\right]\left(\mathscr{B}[\odot]\left(\mathbb{C}_{P}, C_{Q}\right)\right)$ 是选点查询的结果。是多张 canvas。

### 第k近邻居查询

做法：给定一个半径画圆，求出点的数量，据此二分答案。



## 实现注意点

1. 对于点，需要记住精确的 location
2. 对于多边形，需要标记像素是否在边界上
3. 使用扩展：conservative rasterization
4. 维护一个map，将位于边界的像素映射到原多边形（矢量形式）
5. 用 (r,g,b,a) 来储存 canvas function
6. 对于有洞的多边形，先绘制无洞版本，再单独绘制洞，做减法
7. *混合* 通过直接 alpha blending 两张 textures 实现
8. *遮罩* 并行地测试每个像素点。此时边界信息用于精确测试每个像素是否位于多边形边界。



