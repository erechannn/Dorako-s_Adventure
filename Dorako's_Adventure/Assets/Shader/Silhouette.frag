#version 330
layout(location = 0) out vec4 out_FragColor;

// テクスチャ座標
in vec2 v_TexCoord;

// レンダーテクスチャ
uniform sampler2D u_RenderTexture;
// シルエット用のテクスチャ
uniform sampler2D u_SilhouetteTexture;

void main(void) {
	// 元画像のテクスチャを取得
	vec4 color = texture(u_RenderTexture, v_TexCoord);
	// シルエットの情報を取得
	float silhouette = texture(u_SilhouetteTexture, v_TexCoord).r;
	// シルエットの部分はRGB(0.25, 0.25, 0.25)
 	//vec3 finalColor = mix(color.rgb, vec3(0.25, 0.25, 0.25), silhouette); 
	// 元画像の色を残してシルエットの部分だけ暗くしたい場合は下記のようにする
	 vec3 finalColor = mix(color.rgb, color.rgb * 0.25, silhouette); 
	 //最終カラーの出力
	out_FragColor = vec4(vec3(finalColor), 1.0);
}
